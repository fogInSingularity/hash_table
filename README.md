# Hash table

Это `README` посвящено такой структуре данных как `Hash table` и её оптимизации. 

## Выбор функции хеширования

Рассмотрим следующие hash функции:

|Значение хэша|
|-|
|всегда 0|
|всегда первая буква|
|длинна слова|
|сумма букв|
|сумма букв / длинну|
|rol|
|murmur2|

### График заселённости

![graph](bars.png)

### Дисперсия

| hash | дисперсия |
|-|-|
| always zero | 104250 |
| always first char | 3746 |
| length | 121412 |
| char sum | 7469 | 
| normalized | 101 |
| crc | 881 |
| rol | 15 |
| murmur | 10 |

### Результат

Как мы видим `murmur hash` имеет наименьшую дисперсию так что будем использовать именно этот алгоритм.

### Реализация на C

```cpp
static HashValue HashMur(const uint8_t* ukey, size_t len) {
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

Будем проводить замер эффективности `Insert` путем вставки каждого слова из текста в hash таблицу повторив этот процесс 100 раз

| | |
|-|-|
| система | **fedora 39 (linux kernel 6.8.5)** |
| CPU | **amd ryzen 5 3500u** |
| частота | 1.4 GHz |
| температура | ~80 C |
| компилятор | **clang 17.0** |
| опции компилятора | `-O2 -march=znver1 -flto` |
| load factor | ~7 |
| текст | **Bible** (~788 000 слов, ~14 000 уникальных) |
| утиилиты | **perf** и **hotspot** |

## Первоначальная эффективность

### hash insert

![default_speed](/images/default_speed.png)

### strncmp

![default_strncmp_speed](/images/default_strncmp.png)


### hash

![default_hash](/images/default_hash_func.png)

| | циклов затрачено | % от времени |
|-|-|-|
| `hash insert` | $1.495*10^{10}$ | $100$% | 
| `strncmp` | $0.527*10^{10}$ | $35$% |
| `hash` | $0.216*10^{10}$ | $14$% | 

## Оптимизация

Как мы видим основную часть времени заниает `strncmp` и `hash`

### strncmp

Напишем функцию FasterStrncmp на ассмблере с учетом этого:

```mk
FasterStrncmp:
        mov rax, rdx
        dec rax
        and rax, rdx
        test rax, rax
        jne .call_strcmp
        bsr rax, rdx 
        mov rax, [.len_table + 8 * rax]
        jmp rax

.call_strcmp:
        xor rax, rax
        call strncmp
        ret

.len_table:
dq      .pow0 
dq      .pow1
dq      .pow2
dq      .pow3
dq      .pow4
dq      .pow5
dq      .pow6

.pow0:  ; len = 1
        xor rax, rax
        xor r10, r10
        mov al, [rdi]
        mov r10b, [rsi]
        xor rax, r10
        ret

.pow1:  ; len = 2
        xor rax, rax
        xor r10, r10
        mov ax, [rdi]
        mov r10w, [rsi]
        xor rax, r10 ; sub
        ret

.pow2:   ; len = 4
        xor rax, rax
        xor r10, r10
        mov eax, [rdi]
        mov r10d, [rsi]
        xor rax, r10 ; sub
        ret
.pow3:  ; len = 8
        mov rax, [rdi]
        mov r10, [rsi]
        xor rax, r10
        ret
.pow4:  ; len = 16
.pow5:  ; len = 32
.pow6:  ; len = 64
        jmp .call_strcmp
```

