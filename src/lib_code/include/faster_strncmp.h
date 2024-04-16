#ifndef FASTER_STRNCMP_H_
#define FASTER_STRNCMP_H_

#include <stddef.h>
#include "opt.h"

HOT int FasterStrncmp(const char* restrict src1, const char* restrict src2, size_t len);

#endif // FASTER_STRNCMP_H_