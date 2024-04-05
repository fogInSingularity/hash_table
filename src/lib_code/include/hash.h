#ifndef HASH_H_
#define HASH_H_

#include <stddef.h>
#include <stdint.h>

typedef uint32_t HashValue;
HashValue Hash(const uint8_t* key, size_t len, uint32_t seed);

#endif  // HASH_H_