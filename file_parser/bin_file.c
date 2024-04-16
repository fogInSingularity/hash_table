#include "bin_file.h"

#include <stdlib.h>

#include "my_assert.h"

// static-----------------------------------------------------------------------

static size_t FileSize(FILE* file);
static void FileRead(BinData* data, FILE* file);

// global-----------------------------------------------------------------------

void GetData(BinData* data, FILE* file) {
    ASSERT(data != NULL);
    ASSERT(file != NULL);

    data->buf_size = FileSize(file);

    data->buf = (char*)calloc(data->buf_size + 1, sizeof(char));

    FileRead(data, file);
}

void FreeData(BinData* data) {
    ASSERT(data != NULL);

    free(data->buf);
    data->buf = NULL;

    data->buf_size = 0;
}

// static-----------------------------------------------------------------------

void FileRead(BinData* data, FILE* file) {
    ASSERT(data != NULL);
    ASSERT(file != NULL);

    unsigned long ignore = fread(data->buf, sizeof(char), data->buf_size, file);
    (void)ignore;
    data->buf[data->buf_size] = '\0';
}

static size_t FileSize(FILE* file) {
    ASSERT(file != NULL);

    fseek(file, 0, SEEK_END);

    size_t size = (size_t)ftell(file);

    fseek(file, 0, SEEK_SET);

    return size;
}
