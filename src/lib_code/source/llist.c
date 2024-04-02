#include "llist.h"

#include "hash_table_elem.h"
#include "my_assert.h"

// static-----------------------------------------------------------------------

/// @brief list node
typedef struct LLNode {
  HashTElem elem;       ///< node data
  struct LLNode *next;  ///< index of next node
  struct LLNode *prev;  ///< index of previous node
} LLNode;

#define ERROR_M(error_m) fputs(RED BOLD "error: " RESET error_m "\n", stdout);

// public-----------------------------------------------------------------------

LListError LList_Ctor(LList *list) {
  ASSERT(list != NULL);

  list->is_valid = false;
  list->head = NULL;
  list->tail = NULL;

  LLNode *hold_head = (LLNode *)calloc(1, sizeof(LLNode));
  LLNode *hold_tail = (LLNode *)calloc(1, sizeof(LLNode));
  if (hold_head == NULL || hold_tail == NULL) {
    free(NULL);
    free(hold_head);
    free(hold_tail);

    return kLList_CtorCantAlloc;
  }

  list->head = hold_head;
  list->tail = hold_tail;

  list->head->next = list->tail;
  list->head->prev = NULL;

  list->tail->next = NULL;
  list->tail->prev = list->head;

  list->is_valid = true;

  return kLList_Success;
}

void LList_Dtor(LList *list) {
  ASSERT(list != NULL);

  if (!list->is_valid) {
    return;
  }

  LLNode *iter_node = list->head;
  LLNode *next_node = list->head->next;
  while (iter_node != NULL) {
    next_node = iter_node->next;
    free(iter_node);
    iter_node = next_node;
  }

  list->head = NULL;
  list->tail = NULL;
}

LListError LList_Insert(LList *list, StringView *key) {
  ASSERT(list != NULL);
  ASSERT(key != NULL);
}

LListError LList_Remove(LList *list, StringView *key) {
  ASSERT(list != NULL);
  ASSERT(key != NULL);
}

LListError LList_LookUp(LList *list, StringView *key) {
  ASSERT(list != NULL);
  ASSERT(key != NULL);
}