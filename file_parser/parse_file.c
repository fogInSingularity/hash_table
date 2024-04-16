#include "parse_file.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "bin_file.h"
#include "debug.h"
#include "my_assert.h"
#include "my_typedefs.h"
#include "string_view.h"

// static ---------------------------------------------------------------------

const char* SkipPunc(const char* iter_str);
const char* SkipSpaces(const char* iter_str);

void RemoveSpacesAndPunc(BinData* file_data);
FatPointer ParseToWords(BinData* file_data);

// global ---------------------------------------------------------------------

__attribute__((noinline))
FatPointer ParseFile(const int argc, const char** argv, BinData* ret_bin_data) {
    ASSERT(argv != NULL);
    if (argc < 3) { return (FatPointer){NULL, 0}; }
    
    FILE* file = fopen(argv[1], "rb");
    if (file == NULL) { return (FatPointer){NULL, 0}; }
    
    BinData file_data = {};
    GetData(&file_data, file);
    fclose(file);

    RemoveSpacesAndPunc(&file_data);
    FatPointer parse_file = ParseToWords(&file_data);
    if (parse_file.ptr == NULL) { return (FatPointer){NULL, 0}; }

    *ret_bin_data = file_data;

    return parse_file;
}

// static ---------------------------------------------------------------------

const char* SkipPunc(const char* iter_str) {
    ASSERT(iter_str != NULL);

    while (ispunct(*iter_str)) {
        iter_str++;
    }

    return iter_str;
}

const char* SkipSpaces(const char* iter_str) {
    ASSERT(iter_str != NULL);

    while (isspace(*iter_str)) {
        iter_str++;
    }

    return iter_str;
}

void RemoveSpacesAndPunc(BinData* file_data) {
    ASSERT(file_data != NULL);

    char* iter_str = file_data->buf;
    while (iter_str < file_data->buf + file_data->buf_size) {
        if (isspace(*iter_str) || ispunct(*iter_str)) {
            *iter_str = ' ';
        }

        iter_str++;
    }
}

FatPointer ParseToWords(BinData* file_data) {
    ASSERT(file_data != NULL);

    Counter number_words = 0;

    const char* iter_str = file_data->buf;
    while (iter_str < file_data->buf + file_data->buf_size) {
        iter_str = SkipSpaces(iter_str);

        while (!isspace(*iter_str)) {
            iter_str++;
        }
        number_words++;

        iter_str = SkipSpaces(iter_str);
    }
    
    StringView* parsed_words = (StringView*)calloc(number_words, 
                                                   sizeof(StringView));
    if (parsed_words == NULL) {
        return (FatPointer){NULL, 0};
    }
     

    Index iter_arr = 0;
    iter_str = file_data->buf;
    while (iter_str < file_data->buf + file_data->buf_size) {
        iter_str = SkipSpaces(iter_str);
        parsed_words[iter_arr].str = iter_str;
        while (!isspace(*iter_str)) {
            iter_str++;
        }

        parsed_words[iter_arr].len = (size_t)(iter_str - parsed_words[iter_arr].str);
        // PRINT_POINTER(parsed_words[iter_arr].str);
        parsed_words[iter_arr].str = (const char*)(parsed_words[iter_arr].str - file_data->buf);
        // PRINT_POINTER(parsed_words[iter_arr].str);
        iter_arr++;

        iter_str = SkipSpaces(iter_str);
    }

    return (FatPointer){parsed_words, number_words * sizeof(StringView)};
}