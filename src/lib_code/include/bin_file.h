#ifndef LIB_BINFILE_H_
#define LIB_BINFILE_H_

#include <stddef.h>
#include <stdio.h>

typedef struct {
    char* buf;
    size_t buf_size;
} BinData;

void GetData(BinData* data, FILE* file);
void FreeData(BinData* data);

#endif  // LIB_BINFILE_H_
