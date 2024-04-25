#include <stdio.h>
#include <stdlib.h>

#include "bin_file.h"
#include "hash_table.h"
#include "my_typedefs.h"
#include "string_view.h"
#include "load_file.h"
#include "debug.h"

void ClearBeforeTerm(HashTable* hash_table,
                     BinData* file_data, 
                     FatPointer* str_arr);

int main(const int argc, const char** argv) {
    BinData file_data = {0};

    FatPointer str_arr = LoadFile(argc, argv, &file_data);
    if (str_arr.ptr == NULL) { fprintf(stderr, "! Load failed\n"); return -1; }

    HashTable hash_table = {0};
    HashTableError ht_error = kHashTable_Success;

    ht_error = HashTable_Ctor(&hash_table);
    if (ht_error != kHashTable_Success) {
        ClearBeforeTerm(&hash_table, &file_data, &str_arr);
        return 0;
    }

    for (Index i = 0; i < str_arr.size_in_bytes / sizeof(StringView); i++) {
        ht_error = HashTable_InsertByKey(&hash_table,
                                         *((StringView*)str_arr.ptr + i));
        if (ht_error != kHashTable_Success) { 
            fprintf(stderr, "Insertion failed\n"); 
        }
    }

    const Counter number_of_iters = 150;
    for (Counter iter = 0; iter < number_of_iters; iter++) {
        Counter num_in_text = 0; 
        for (Index i = 0; i < str_arr.size_in_bytes / sizeof(StringView); i++) {
            num_in_text = HashTable_LookUpByKey(&hash_table,
                                                *((StringView*)str_arr.ptr + i));
            if (num_in_text == kHashTable_LookUpErr) {
                fprintf(stderr, "LookUp failed\n"); 
            }
        }
    }

    // HashTable_Dump(&hash_table);
    ClearBeforeTerm(&hash_table, &file_data, &str_arr);
}

void ClearBeforeTerm(HashTable* hash_table,
                     BinData* file_data,
                     FatPointer* str_arr) {
    HashTable_Dtor(hash_table);
    free(str_arr->ptr);
    FreeData(file_data);
}