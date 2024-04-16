#ifndef HASH_H_
#define HASH_H_

#include <stddef.h>
#include <stdint.h>

#include "opt.h"

typedef uint32_t HashValue;

HOT HashValue Hash(const char* key, size_t len);

#endif  // HASH_H_