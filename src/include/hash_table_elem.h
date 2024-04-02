#ifndef HASH_TABLE_ELEM_H_
#define HASH_TABLE_ELEM_H_

#include "hash.h"
#include "my_typedefs.h"
#include "string_view.h"

typedef struct HashTElem {
    // HashValue hash_value;
    Counter n_copies;
    StringView string;
} HashTElem;

#endif // HASH_TABLE_ELEM_H_