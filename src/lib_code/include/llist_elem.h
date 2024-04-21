#ifndef LLIST_ELEM_H_
#define LLIST_ELEM_H_

#include "my_typedefs.h"

#include "llist_config.h"

typedef struct LListElem {
    char word_key[kWordMaxLen];
    Counter n_copies;
} LListElem;

#endif // LLIST_ELEM_H_