#include <stdio.h>
#include <stddef.h>

#include "mmap_wraper.h"
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
static size_t FileDescrSize(FileDescr file_descr);
static void* MapFile(FileDescr file_descr, size_t file_size_in_bytes);

// global ----------------------------------------------------------------------

FatPointer LoadFile(const int argc, 
                    const char** argv, 
                    BinData* file_data, 
                    int* mapped_fd) {
    ASSERT(argv != NULL);
    ASSERT(file_data != NULL);
    
    if (argc < 3) { return (FatPointer){NULL, 0}; }

    FILE* file_text = FOpenW(argv[1], "r"); $
    if (file_text == NULL) { return (FatPointer){NULL, 0}; }

    GetData(file_data, file_text); $
    FCloseW(file_text);

    FileDescr parsed_file_descr = RawOpenW(argv[2], O_RDWR); $
    *mapped_fd = parsed_file_descr;
    if (parsed_file_descr == -1) { 
    #ifdef DEBUG
        perror("Cant open file\n");
    #endif // NDEBUG
        return (FatPointer){NULL, 0}; 
    }

    FatPointer loaded_file = {0}; $
    loaded_file.size_in_bytes = FileDescrSize(parsed_file_descr); $
    loaded_file.ptr = MapFile(parsed_file_descr, loaded_file.size_in_bytes); $

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

static size_t FileDescrSize(FileDescr file_descr) {
    ASSERT(file_descr != -1);

    struct stat file_stat = {0};
    fstat(file_descr, &file_stat);

    return file_stat.st_size;
}

static void* MapFile(FileDescr file, size_t file_size_in_bytes) {
    ASSERT(file != -1);

    void* maped_file = mmap(NULL, 
                            file_size_in_bytes, 
                            PROT_WRITE | PROT_READ, 
                            MAP_SHARED, 
                            file, 
                            0);
 
    return maped_file;
}