# Hash table

Это `README` посвящено такой структуре данных как `Hash table` и её оптимизации.

## Постановка задачи

Создание и исследование системы которая, получив текстовый файл, может ответить сколько раз встречается данное слово. Для этого будем использовать hash-таблицу.

## Немного о Hash table

Хеш таблица - структура данных, реализующая интерфейс ассоциативного массива. Позволяет по ключу получить доступ к значению ключа. Основным отличием является использование хеш-функции (функция которая соотносит последовательности бит числовое значение, чаще всего не взаимнооднозначно), для доступа к значению.

## Hash table API

`hash_table.h`

```cpp
HashTableError HashTable_Ctor(HashTable* hash_table);
void HashTable_Dtor(HashTable* hash_table);

HashTableError HashTable_InsertByKey(HashTable* hash_table,
                                     const StringView key);
HashTableError HashTable_RemoveByKey(HashTable* hash_table,
                                     const StringView key);
Counter HashTable_LookUpByKey(const HashTable* hash_table,
                              const StringView key);
```

```cpp
struct StringView {
    const char* str;
    size_t len;
};

```

## Выбор функции хеширования

Рассмотрим следующие hash-функции:

|Значение хеша      |
|-------------------|
|всегда 0           |
|всегда первая буква|
|длина слова       |
|сумма букв         |
|(сумма букв) / длину |
|crc32              |
|rol                |
|murmur2            |

### График заселённости

![graph](/images/bars.png)

### Дисперсия

| hash              | дисперсия    |        |
|-------------------|--------------|--------|
| always zero       | $1.0 * 10^5$ | 100000 |
| length            | $1.2 * 10^4$ | 12000  |
| char sum          | $7.5 * 10^3$ | 7500   |
| always first char | $3.7 * 10^3$ | 3700   |
| normalized        | $1.0 * 10^2$ | 100    |
| rol               | $1.5 * 10^1$ | 15     |
| murmur            | $1.0 * 10^1$ | 10     |
| crc32             | $7.3 * 10^0$ | 7.3    |

### Результат

Как мы видим `crc32` имеет наилучшую дисперсию так что будем использовать именно этот алгоритм.

### Реализация на C

```cpp
uint32_t HashCRC(const char* key, size_t len) {
    uint64_t crc32 = 0xbebeb0ba;
    if (len == 0) { return crc32; }

    for (Index i = 0; i < len; i++) {
        __asm__ volatile ("crc32 %0, byte ptr [%1]\n"
                          : "=r" (crc32)
                          : "r" (key), "r" (crc32));
        key++;
    }

    return crc32;
}
```

## Детали

Будем проводить замер эффективности `LookUp` путем вставки каждого слова из текста в hash таблицу, после чего 150 раз сделаем `LookUp` каждого слова из текста.

|                    |                                               |
|--------------------|-----------------------------------------------|
| система            | **fedora 40 (linux kernel 6.8.7)**            |
| CPU                | **amd ryzen 5 3500u**                         |
| частота            | **1.4 GHz**                                   |
| температура        | **~80 C**                                     |
| компилятор         | **clang 18.1**                                |
| опции компилятора  | `-O2 -march=znver1 -flto`                     |
| load factor        | **~7**                                        |
| количество бакетов | **2003**                                      |
| текст              | **Bible** (~788 000 слов, ~14 000 уникальных) |
| утиилиты           | **[perf](https://perf.wiki.kernel.org/index.php/Main_Page)**, **[hotspot](https://github.com/KDAB/hotspot)** и **[hyperfine](https://github.com/sharkdp/hyperfine?tab=readme-ov-file) (--runs=20 --warmup=5)** |

## Первоначальная эффективность

![noop](/images/noop.png)

|                | циклов затрачено | % от времени |
|----------------|------------------|--------------|
| `main`         | $1.881*10^{10}$  | $100$%       |
| `strcmp`       | $0.740*10^{10}$  | $39.3$%      |
| `Hash`         | $0.151*10^{10}$  | $8.1$%       |
| `AccessBucket` | $0.170*10^{10}$  | $9.0$%       |

Дальнейшее сравнение времени будем проводить по времени всей программы с помощью **hyperfine**

Время без оптимизаций:
**$(12.8 \pm 0.7)$ s**

## Оптимизация

Как мы видим основную часть времени занимает `strncmp`, `Hash` и `AccessBucket`

### strncmp

Слова в тексте не превышают $32$ символов. Так же получая `const StringView` мы копируем содержимое строки в буфер выровненный по $32$. Всё это позволит использовать **avx2** и **aligned load**:

```cpp
#include <immintrin.h>

#define kWordMaxLen 32

int FasterStrcmp(const char word_key1[kWordMaxLen],
                 const char word_key2[kWordMaxLen]) {
    __m256i load1 = _mm256_load_si256((const __m256i*)word_key1);
    __m256i load2 = _mm256_load_si256((const __m256i*)word_key2);
    __m256i xor_vec = _mm256_xor_si256(load1, load2);

    return !_mm256_testz_si256(xor_vec, xor_vec);
}
```

Время при оптимизированной `strcmp`:
**$(10.554 \pm 0.022)$ s**, прирост производительности на $21$% (в сравнении с реализацией без оптимизаций)

### Hash

В первоначальной реализации каждый символ хэшировался поочередно, однако мы  можоем векторизировать данный процесс.

Векторизируем по 2 элемента:
```c

__attribute__((unused))
uint32_t HashCRCFast(const char* key, size_t len) {
    uint32_t crc32 = 0;
    if (len == 0) { return crc32; }

    for (Index i = 0; i < len >> 1; i++) {
        __asm__ volatile ("crc32 %0, word ptr [%1]\n"
                          : "=r" (crc32)
                          : "r" (key), "r" (crc32));
        key += sizeof(uint16_t);
    }

    if (trailer & 0b1) {
        __asm__ volatile ("crc32 %0, byte ptr [%1]\n"
                          : "=r" (crc32)
                          : "r" (key), "r" (crc32));
        key += sizeof(uint8_t);
    }

    return crc32;
}
```

Стоит отметить, что хэширование не по 2 символа за раз а например по 8, приводит к частой коллизии, причину которой я не смог выяснить.

Время при оптимизированных `strcmp + Hash`:
**$(9.74 \pm 0.06)$ s**, прирост производительности на $8$% (в сравнении с оптимизацией только strcmp).

### AccessBucket

Функция выглядит следующим образом:

```cpp
LList* AccessBucket(const HashTable* hash_table, uint32_t hash_value) {
    return hash_table->buckets + (hash_value % hash_table->n_buckets);
}
```
Статистика **perf**:

![acb_problem](images/acb_problem.png)

Как мы видим ~$75$% времени функции занимает `div rdi`, полученная из `hash_value % hash_table->n_buckets`

Операция div является достаточно дорогой, **~41 cpu cycle**, в сравнении с более простыми арифметическими операциями, которые выполняются за **~1 cpu cycle**

В нашей реализации, размер hash-таблицы фиксирован ( =$2003$ для данного текста ).
При фиксированном значение модуля мы можем представить его как комбинацию сложения, умножения и сдвигов. Для этого чаще всего используется техника называемая Barrett reduction.

Более того, компилятор обладает этой техникой и сам может оптимизировать `%`, но в данной реализации вторым операндом является поле структуры, которое хранит $2003$ и так как поле n_buckets не является `const`, компилятор не может выполнить эту оптимизацию.

Заменим `hash_table->n_buckets` на константу:
```cpp
LList* AccessBucket(const HashTable* hash_table, uint32_t hash_value) {
    return hash_table->buckets + (hash_value % kNumberOfBuckets);
}
```

Результат:

![acb_noprob](/images/acb_noprob.png)

Время при оптимизированных `strcmp + Hash + AccessBucket`:
**$(9.73 \pm 0.04)$ s**, что не отличается от предыдущего результата.

<!-- //FIXME barret reduction -->

## Результат всех оптимизаций

В результате всех оптимизаций время с **$(12.8 \pm 0.7)$ s** уменьшилось до **$(9.73 \pm 0.04)$ s**

Таким образом мы добились прирост производительности в **31%**.

## References

[MurMurHash2](https://ru.wikipedia.org/wiki/MurmurHash2)

[intel intrinsics guide](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html)

[Barrett reduction](https://en.wikipedia.org/wiki/Barrett_reduction)

## Contact me

naumov.vn@phystech.edu
