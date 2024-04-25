#include <stdio.h>
#include <stddef.h>

#include "file_wraper.h"
#include "load_file.h"
#include "my_assert.h"
#include "my_typedefs.h"
#include "bin_file.h"
#include "string_view.h"
#include "debug.h"

// static ----------------------------------------------------------------------

static void AddToStrViewByPoint(FatPointer* loaded_file, 
                                Index i, 
                                const char* str);

// global ----------------------------------------------------------------------

FatPointer LoadFile(const int argc, 
                    const char** argv, 
                    BinData* file_data) {
    ASSERT(argv != NULL);
    ASSERT(file_data != NULL);
    
    if (argc < 3) { return (FatPointer){NULL, 0}; }

    FILE* file_text = FOpenW(argv[1], "r"); $
    if (file_text == NULL) { return (FatPointer){NULL, 0}; }

    GetData(file_data, file_text); $
    FCloseW(file_text);

    FILE* parsed_file = FOpenW(argv[2], "r");
    if (parsed_file == NULL) { return (FatPointer){NULL, 0}; }
    BinData parsed_data = {0};
    GetData(&parsed_data, parsed_file);
    FCloseW(parsed_file);

    FatPointer loaded_file = {0}; $
    loaded_file.ptr = parsed_data.buf;
    loaded_file.size_in_bytes = parsed_data.buf_size;
    
    for (Index i = 0; i < loaded_file.size_in_bytes / sizeof(StringView); i++) {
        AddToStrViewByPoint(&loaded_file, i, file_data->buf);
    }

    return loaded_file;
}

// static ----------------------------------------------------------------------

static void AddToStrViewByPoint(FatPointer* loaded_file, 
                                Index i, 
                                const char* str) {
    ASSERT(loaded_file != NULL);
    ASSERT(str != NULL);

    StringView* ptr = (StringView*)(loaded_file->ptr + (sizeof(StringView) * i));
    ptr->str += (size_t)str;
}