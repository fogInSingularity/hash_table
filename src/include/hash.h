#ifndef HASH_H_
#define HASH_H_

#include <stddef.h>
#include <stdint.h>

typedef uint32_t HashValue;

HashValue Hash(const char* key, size_t len);

#endif  // HASH_H_