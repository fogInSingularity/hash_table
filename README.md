# Hash table

Это `README` посвящено такой структуре данных как `Hash table` и её оптимизации. 

## Постановка задачи

Создание и исследование системы которая, получив текстовый файл, может ответить сколько раз встречается данное слово. Для этого будем использовать hash-таблицу.

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

Рассмотрим следующие hash функции:

|Значение хеша      |
|-------------------|
|всегда 0           |
|всегда первая буква|
|длинна слова       |
|сумма букв         |
|сумма букв / длину |
|rol                |
|murmur2            |

### График заселённости

![graph](/images/bars.png)

### Дисперсия

| hash              | дисперсия    |
|-------------------|--------------|
| always zero       | $10^5$       |
| length            | $1.2 * 10^5$ |
| char sum          | $7.5 * 10^3$ | 
| always first char | $3.7 * 10^3$ |
| crc               | $8 * 10^2$   |
| normalized        | $10^2$       |
| rol               | 15           |
| murmur            | 10           |

### Результат

Как мы видим `murmur hash` имеет наименьшую дисперсию так что будем использовать именно этот алгоритм.

### Реализация на C

```cpp
uint32_t HashMur(const uint8_t* ukey, size_t len) {
    uint32_t h = 0xbebeb0ba;
    uint32_t k = 0;

    for (size_t i = len >> 2; i; i--) {
        memcpy(&k, ukey, sizeof(uint32_t));

        ukey += sizeof(uint32_t);

        h ^= HashScramble(k);
        h = (h << 13) | (h >> 19);
        h = h * 5 + 0xe6546b64;
    }

    k = 0;
    for (size_t i = len & 3; i != 0; i--) {
        k <<= 8;
        k |= ukey[i - 1];
    }

    h ^= HashScramble(k);

    h ^= (uint32_t)len;
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;

    return h;
}

inline uint32_t HashScramble(uint32_t k) {
    k *= 0xcc9e2d51;
    k = (k << 15) | (k >> 17);
    k *= 0x1b873593;

    return k;
}
```

## Детали 

Будем проводить замер эффективности `LookUp` путем вставки каждого слова из текста в hash таблицу, после чего 30 раз сделаем `LookUp` каждого слова из текста.

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
| утиилиты           | **[perf](https://perf.wiki.kernel.org/index.php/Main_Page)**, **[hotspot](https://github.com/KDAB/hotspot)** и **[hyperfine](https://github.com/sharkdp/hyperfine?tab=readme-ov-file)**         |

## Первоначальная эффективность

### main

![noop_main](/images/noop_main.png)

### strcmp

![noop_strcmp](/images/noop_strcmp.png)

### hash

![noop_hash](/images/noop_hash.png)

### access bucket

![noop_acb](/images/noop_acb.png)

|                | циклов затрачено | % от времени |
|----------------|------------------|--------------|
| `main`         | $4.392*10^{9}$   | $100$%       | 
| `strcmp`       | $1.575*10^{9}$   | $35.8$%      |
| `Hash`         | $0.705*10^{9}$   | $16.0$%      | 
| `AccessBucket` | $0.465*10^{9}$   | $10.6$%      |

Дальнейшее сравнение времени будем проводить по времени всей программы с помощью **hyperfine**

Время без оптимизаций:
**$(4.829 \pm 0.233)$ s**

## Оптимизация

Как мы видим основную часть времени занимает `strncmp`, `Hash` и `AccessBucket`

### strncmp

Слова в тексте не превышают 32 символов. Так же получая `const StringView*` мы копируем содержимое строки в буфер выровненный по 32. Всё это позволит использовать avx2 и выроненный load:

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
**$(4.003 \pm 0.042)$ s**

### Hash

Взглянем на ассемблер который порождает **clang 17.0** для функции хеша:
```cpp
HashMur:                                ;# @HashMur
        mov     eax, -1094799174
        cmp     rsi, 4
        jb      .LBB0_4
        mov     rdx, rsi
        mov     rcx, rsi
        xor     r8d, r8d
        shr     rdx, 2
        and     rcx, -4
.LBB0_2:                                ;# =>This Inner Loop Header: Depth=1
        mov     r9d, dword ptr [rdi + 4*r8]
        inc     r8
        imul    r10d, r9d, -862048943
        imul    r9d, r9d, 380141568
        shr     r10d, 17
        or      r10d, r9d
        imul    r9d, r10d, 461845907
        xor     r9d, eax
        rorx    eax, r9d, 19
        lea     eax, [rax + 4*rax - 430675100]
        cmp     rdx, r8
        jne     .LBB0_2
        add     rdi, rcx
.LBB0_4:
        mov     rcx, rsi
        and     rcx, 3
        je      .LBB0_5
        xor     r8d, r8d
.LBB0_8:                                ;# =>This Inner Loop Header: Depth=1
        movzx   edx, byte ptr [rdi + rcx - 1]
        shl     r8d, 8
        or      edx, r8d
        dec     rcx
        mov     r8d, edx
        jne     .LBB0_8
        jmp     .LBB0_6
.LBB0_5:
        xor     edx, edx
.LBB0_6:
        imul    ecx, edx, -862048943
        imul    edx, edx, 380141568
        xor     eax, esi
        shr     ecx, 17
        or      ecx, edx
        imul    ecx, ecx, 461845907
        xor     eax, ecx
        mov     ecx, eax
        shr     ecx, 16
        xor     ecx, eax
        imul    eax, ecx, -2048144789
        mov     ecx, eax
        shr     ecx, 13
        xor     ecx, eax
        imul    ecx, ecx, -1028477387
        mov     eax, ecx
        shr     eax, 16
        xor     eax, ecx
        ret
```

Как мы видим основными операциями являются: 

1) Арифметические операции: `imul`, `xor`, `or`, `shl`, `shr`, `and`
2) Условные переходы: `jne`, `je`, `jb`
3) mov операторы: `mov`, `movzx`

"Медленными" среди них являются условные переходы и mov операторы с памятью

Мы можем уменьшить количество условных переходов до 1 с помощью джамп-таблицы
[hash_asm.asm](/src/source/hash_asm.asm)

Время при оптимизированных `strcmp + Hash`:
**$(3.940 \pm 0.037)$ s**

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

В нашей реализации, размер хеш-таблицы фиксирован ( =2003 для данного текста ).
При фиксированном значение модуля мы можем представить его как комбинацию сложения, умножения и сдвигов. Для этого чаще всего используется техника называемая Barrett reduction.

Более того, компилятор обладает этой техникой и сам может оптимизировать `%`, но в данной реализации вторым операндом является поле структуры, которое хранит 2003 и так как поле n_buckets не является `const`, компилятор не может выполнить эту оптимизацию.

Заменим `hash_table->n_buckets` на константу:
```cpp
LList* AccessBucket(const HashTable* hash_table, uint32_t hash_value) {
    return hash_table->buckets + (hash_value % kNumberOfBuckets);
}
```

Результат:

![acb_noprob](/images/acb_noprob.png)


Время при оптимизированных `strcmp + Hash + AccessBucket`:
**$(3.926 \pm 0.043)$ s**

<!-- //FIXME barret reduction -->

## Результат всех оптимизаций

В результате всех оптимизаций время с **$(4.829 \pm 0.233)$ s** уменьшилось до **$(3.926 \pm 0.043)$ s**

Прирост производительности составляет **19%**.

## References

[MurMurHash2](https://ru.wikipedia.org/wiki/MurmurHash2)

[intel intrinsics guide](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html)

[Barrett reduction](https://en.wikipedia.org/wiki/Barrett_reduction)

## Contact me

naumov.vn@phystech.edu