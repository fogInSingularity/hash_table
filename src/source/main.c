#include <stdio.h>
#include <stdlib.h>

#include "bin_file.h"
#include "debug.h"
#include "hash_table.h"
#include "my_typedefs.h"
#include "parse_file.h"
#include "string_view.h"

int main(const int argc, const char** argv) {
    BinData file_data = {0};

    FatPointer str_arr = ParseFile(argc, argv, &file_data);
    if (str_arr.ptr == NULL) {
        return -1;
    }

    HashTable hash_table = {0};
    HashTableError ht_error = kHashTable_Success;

    ht_error = HashTable_Ctor(&hash_table);
    if (ht_error == kHashTable_Success) {
        for (Index i = 0; i < str_arr.size_in_bytes / sizeof(StringView); i++) {
            ht_error = HashTable_InsertByKey(&hash_table,
                                             (StringView*)str_arr.ptr + i);
            if (ht_error != kHashTable_Success) { printf("oh shit, here we go again\n"); }
        }
    }

    // HashTable_Dump(&hash_table);
    HashTable_Dtor(&hash_table);

    free(str_arr.ptr);
    FreeData(&file_data);
}