#ifndef LLIST_H_
#define LLIST_H_

#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>
#include <stddef.h>

#include "debug.h"
#include "hash_table_elem.h"
#include "my_assert.h"
#include "my_typedefs.h"
#include "string_view.h"

typedef enum LListError {
  kLList_Success = 0,
  kLList_CtorCantAlloc = 1,
} LListError;

typedef struct LList {
  bool is_valid;
  struct LLNode* head;
  struct LLNode* tail;
} LList;

LListError LList_Ctor(LList* list);
void LList_Dtor(LList* list);
void LList_ThrowError(LListError error);

LListError LList_Insert(LList* list, StringView* key);
LListError LList_Remove(LList* list, StringView* key);
LListError LList_LookUp(LList* list, StringView* key);

#endif  // LLIST_H_
