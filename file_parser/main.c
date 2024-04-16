#include <stdlib.h>
#include <stdio.h>

#include "bin_file.h"
#include "debug.h"
#include "my_typedefs.h"
#include "parse_file.h"
#include "bin_file.h"

int main(const int argc, const char** argv) {
    BinData file_data = {0};

    FatPointer str_arr = ParseFile(argc, argv, &file_data);
    if (str_arr.ptr == NULL) { 
        fprintf(stderr, "! Unable to parse\n");
        return -1; 
    }

    FILE* file_to_store = fopen(argv[2], "w");
    if (file_to_store == NULL) {
        FreeData(&file_data);
        fprintf(stderr, "! Unable to open file\n");
        return -1;
    }

    fwrite(str_arr.ptr, str_arr.size_in_bytes, 1, file_to_store);

    FreeData(&file_data);
    free(str_arr.ptr);

    return 0.0;
}