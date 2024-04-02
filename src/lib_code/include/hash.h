#ifndef HASH_H_
#define HASH_H_

#include <stdint.h>
#include <stddef.h>
#include <string.h>

typedef uint32_t HashValue;
HashValue Hash(const uint8_t* key, size_t len, uint32_t seed);

#endif // HASH_H_