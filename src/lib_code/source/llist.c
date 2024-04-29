#include "llist.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "llist_config.h"
#include "llist_elem.h"
#include "my_assert.h"
#include "faster_strcmp.h"

#include "opt.h"

// static ----------------------------------------------------------------------

typedef struct LLNode {
    LListElem elem;
    struct LLNode* next;
    struct LLNode* prev;
} LLNode;

// #define ERROR_M(error_m) fputs(RED BOLD "error: " RESET error_m "\n", stdout);

HOT static bool CompareKeysEqual(const char word_key1[kWordMaxLen],
                                 const char word_key2[kWordMaxLen]);

// global ----------------------------------------------------------------------

LListError LList_Ctor(LList* list) {
    ASSERT(list != NULL);

    list->is_valid = false;
    list->n_items = 0;
    list->head = NULL;
    list->tail = NULL;

    LLNode* hold_head = (LLNode*)calloc(1, sizeof(LLNode));
    LLNode* hold_tail = (LLNode*)calloc(1, sizeof(LLNode));
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

void LList_Dtor(LList* list) {
    ASSERT(list != NULL);

    if (!list->is_valid) {
        return;
    }

    LLNode* iter_node = list->head;
    LLNode* next_node = NULL;
    while (iter_node != NULL) {
        next_node = iter_node->next;
        free(iter_node);
        iter_node = next_node;
    }

    list->head = NULL;
    list->tail = NULL;
    list->is_valid = false;
    list->n_items = 0;
}

LListError LList_Insert(LList* list, const char word_key[kWordMaxLen]) {
    ASSERT(list != NULL);

#if defined (LLIST_SAFE_CHECKS)
    if (list->is_valid) UNLIKELY {
        return kLList_InvalidLList; $
    }
#endif // LLIST_SAFE_CHECKS

    LLNode* iter_node = list->head->next;
    bool is_keys_equal = false;
    while (!is_keys_equal && iter_node != list->tail) {
        is_keys_equal = CompareKeysEqual(iter_node->elem.word_key, word_key);
        if (!is_keys_equal) {
            iter_node = iter_node->next;
        }
    }

    if (is_keys_equal) LIKELY {
        iter_node->elem.n_copies++;
    } else {
        LLNode* new_node = (LLNode*)aligned_alloc(32, 64); //FIXME
        if (new_node == NULL) UNLIKELY { return kLList_BadInsert; }
        memset(new_node, 0, sizeof(LLNode));

        new_node->elem.n_copies = 1;
        strncpy(new_node->elem.word_key, word_key, kWordMaxLen);

        new_node->next = list->tail;
        new_node->prev = list->tail->prev;
        new_node->prev->next = new_node;
        list->tail->prev = new_node;

        list->n_items++;
    }

    return kLList_Success;
}

LListError LList_Remove(LList* list, const char word_key[kWordMaxLen]) {
    ASSERT(list != NULL);

#if defined (LLIST_SAFE_CHECKS)
    if (!list->is_valid) UNLIKELY {
        return kLList_InvalidLList; $
    }
#endif // LLIST_SAFE_CHECKS

    LLNode* iter_node = list->head->next;
    bool is_keys_equal = false;
    while (iter_node != list->tail && !is_keys_equal) {
        is_keys_equal = CompareKeysEqual(iter_node->elem.word_key, word_key);
        if (!is_keys_equal) {
            iter_node = iter_node->next;
        }
    }

    if (is_keys_equal) {
        LLNode* hold_node = iter_node;
        hold_node->next->prev = hold_node->prev;
        hold_node->prev->next = hold_node->next;
        free(hold_node);

        list->n_items--;
    } else {
        return kLList_NoElementToRemove;
    }

    return kLList_Success;
}

Counter LList_LookUp(LList* list, const char word_key[kWordMaxLen]) {
    ASSERT(list != NULL);

#if defined (LLIST_SAFE_CHECKS)
    if (!list->is_valid) {
        return kLList_LookUpErr; $
    }
#endif // LLIST_SAFE_CHECKS

    LLNode* iter_node = list->head->next;
    bool is_keys_equal = false;
    while (iter_node != list->tail && !is_keys_equal) {
        is_keys_equal = CompareKeysEqual(iter_node->elem.word_key, word_key);
        if (!is_keys_equal) {
            iter_node = iter_node->next;
        }
    }

    return is_keys_equal
           ? iter_node->elem.n_copies
           : 0;
}

// static ----------------------------------------------------------------------

static bool CompareKeysEqual(const char word_key1[kWordMaxLen],
                             const char word_key2[kWordMaxLen]) {
    // int first_check = strcmp(word_key1, word_key2);
    int first_check = FasterStrcmp(word_key1, word_key2);

    return first_check == 0 ? true : false;
}
