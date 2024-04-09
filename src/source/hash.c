#include "hash.h"

#include <stdint.h>
#include <memory.h>
#include <assert.h>

#include "debug.h"
#include "my_assert.h"
#include "hash_table_cfg.h"
#include "my_typedefs.h"

// static ---------------------------------------------------------------------

static const size_t kBitsInByte = 8;

static uint32_t HashScramble(uint32_t k);
// __attribute__((unused))
// static HashValue RorN(HashValue hash_value, size_t n);
static HashValue Ror(HashValue hash_value);
static HashValue Rol(HashValue hash_value);

__attribute__((unused))
static HashValue HashAlwaysZero(const char* key, size_t len);
__attribute__((unused))
static HashValue HashFirstChar(const char* key, size_t len);
__attribute__((unused))
static HashValue HashLength(const char* key, size_t len);
__attribute__((unused))
static HashValue HashCharSum(const char* key, size_t len);
__attribute__((unused))
static HashValue HashNorm(const char* key, size_t len);
__attribute__((unused))
static HashValue HashRor(const char* key, size_t len);
__attribute__((unused))
static HashValue HashRol(const char* key, size_t len);
__attribute__((unused))
static HashValue HashBased(const char* key, size_t len);

// global ---------------------------------------------------------------------

__attribute__((noinline))
HashValue Hash(const char* key, size_t len) {
    ASSERT(key != NULL);

    #if defined (HASH_ALWAYS_ZERO)
        return HashAlwaysZero(key, len);
    #elif defined (HASH_FIRST_CHAR)
        return HashFirstChar(key, len);
    #elif defined (HASH_LENGTH)
        return HashLength(key, len);
    #elif defined (HASH_CHAR_SUM)
        return HashCharSum(key, len);
    #elif defined (HASH_NORM)
        return HashNorm(key, len);
    #elif defined (HASH_ROR)
        return HashRor(key, len);
    #elif defined (HASH_ROL)
        return HashRol(key, len);
    #elif defined (HASH_BASED)
        return HashBased(key, len);
    #else 
        static_assert(0, "! No hash functions were defined");
    #endif
}

// static ---------------------------------------------------------------------

__attribute__((unused))
static HashValue HashAlwaysZero(const char* key, size_t len) {
    ASSERT(key != NULL);
    USE_VAR(key);
    USE_VAR(len);
    
    return 0;
}

__attribute__((unused))
static HashValue HashFirstChar(const char* key, size_t len) {
    ASSERT(key != NULL);

    return len != 0 ? (HashValue)key[0] : 0;
}

__attribute__((unused))
static HashValue HashLength(const char* key, size_t len) {
    ASSERT(key != NULL);
    USE_VAR(key);

    return (HashValue)len;
}

__attribute__((unused))
static HashValue HashCharSum(const char* key, size_t len) {
    ASSERT(key != NULL);

    HashValue hash_value = 0;

    for (Index i = 0; i < len; i++) {
        hash_value += (HashValue)key[i];
    }

    return hash_value;
}

__attribute__((unused))
static HashValue HashNorm(const char* key, size_t len) {
    return HashCharSum(key, len) / (HashValue)len;
}

__attribute__((unused))
static HashValue HashRor(const char* key, size_t len) {
    ASSERT(key != NULL);

    HashValue hash_value = 0;

    for (Index i = 0; i < len; i++) {
        hash_value = Ror(hash_value) ^ (HashValue)key[i];
    }

    return hash_value;
}

__attribute__((unused))
static HashValue HashRol(const char* key, size_t len) {
    ASSERT(key != NULL);

    HashValue hash_value = 0;

    for (Index i = 0; i < len; i++) {
        hash_value = Rol(hash_value) ^ (HashValue)key[i];
    }

    return hash_value;
}

__attribute__((unused))
static HashValue HashBased(const char* key, size_t len) {
    ASSERT(key != NULL);

    const uint8_t* ukey = (const uint8_t*)key;
    
    uint32_t h = 0;
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

static inline uint32_t HashScramble(uint32_t k) {
    k *= 0xcc9e2d51;
    k = (k << 15) | (k >> 17);
    k *= 0x1b873593;

    return k;
}

// __attribute__((unused))
// static HashValue RorN(HashValue hash_value, size_t n) {
//     n = n % sizeof(hash_value) * kBitsInByte;

//     return (hash_value >> n) | (hash_value << (sizeof(hash_value) * kBitsInByte - n));
// }

static HashValue Ror(HashValue hash_value) {
    return (hash_value >> 1) | (hash_value << (sizeof(hash_value) * kBitsInByte - 1));
}

static HashValue Rol(HashValue hash_value) {
    return (hash_value << 1) | (hash_value >> (sizeof(hash_value) * kBitsInByte - 1));
}