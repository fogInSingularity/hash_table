#ifndef LLIST_H_
#define LLIST_H_

#include <memory.h>
#include <stdbool.h>
#include <stdint.h>

#include "llist_config.h"
#include "my_typedefs.h"
#include "string_view.h"

typedef enum LListError {
    kLList_Success           = 0,
    kLList_CtorCantAlloc     = 1,
    kLList_InvalidLList      = 2,  // realy bad
    kLList_BadInsert         = 3,
    kLList_NoElementToRemove = 4,
} LListError;
static const Counter kLList_LookUpErr = UINT64_MAX;

typedef struct LList {
    bool is_valid;
    size_t n_items;
    struct LLNode* head;
    struct LLNode* tail;
} LList;

LListError LList_Ctor(LList* list);
void LList_Dtor(LList* list);
void LList_ThrowError(LListError error);

LListError LList_Insert(LList* list, const char word_key[kWordMaxLen]);
LListError LList_Remove(LList* list, const char word_key[kWordMaxLen]);
Counter LList_LookUp(LList* list, const char word_key[kWordMaxLen]);

#endif  // LLIST_H_
