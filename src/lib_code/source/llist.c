#include "llist.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "hash_table_elem.h"
#include "my_assert.h"
#include "my_typedefs.h"
#include "string_view.h"
#include "utils.h"
#include "faster_strncmp.h"

#include "opt.h"

// static ---------------------------------------------------------------------

typedef struct LLNode {
    HashTElem elem;
    struct LLNode* next;
    struct LLNode* prev;
} LLNode;

#define ERROR_M(error_m) fputs(RED BOLD "error: " RESET error_m "\n", stdout);

HOT static bool CompareKeysEqual(StringView* key1, StringView* key2);

// global ---------------------------------------------------------------------

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

__attribute__((noinline))
LListError LList_Insert(LList* list, StringView* key) {
    ASSERT(list != NULL);
    ASSERT(key != NULL);

    if (!list->is_valid) UNLIKELY {
        return kLList_InvalidLList; $
    }

    LLNode* iter_node = list->head->next;
    bool is_keys_equal = false;
    while (!is_keys_equal && iter_node != list->tail) {
        is_keys_equal = CompareKeysEqual(&iter_node->elem.string, key);
        if (!is_keys_equal) {
            iter_node = iter_node->next;
        }
    }

    if (is_keys_equal) LIKELY { //optim
        iter_node->elem.n_copies++;
    } else {
        LLNode* new_node = (LLNode*)calloc(1, sizeof(LLNode));
        if (new_node == NULL) UNLIKELY {
            return kLList_BadInsert;
        }

        new_node->elem.n_copies = 1;
        new_node->elem.string = *key;

        new_node->next = list->tail;
        new_node->prev = list->tail->prev;
        new_node->prev->next = new_node;
        list->tail->prev = new_node;

        list->n_items++;
    }

    return kLList_Success;
}

LListError LList_Remove(LList* list, StringView* key) {
    ASSERT(list != NULL);
    ASSERT(key != NULL);

    if (!list->is_valid) {
        return kLList_InvalidLList; $
    }

    LLNode* iter_node = list->head->next;
    bool is_keys_equal = false;
    while (iter_node != list->tail && !is_keys_equal) {
        is_keys_equal = CompareKeysEqual(&iter_node->elem.string, key);
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

ErrorCounter LList_LookUp(LList* list, StringView* key) {
    ASSERT(list != NULL);
    ASSERT(key != NULL);

    if (!list->is_valid) {
        return -1; $
    }

    LLNode* iter_node = list->head->next;
    bool is_keys_equal = false;
    while (iter_node != list->tail && !is_keys_equal) {
        is_keys_equal = CompareKeysEqual(&iter_node->elem.string, key);
        if (!is_keys_equal) {
            iter_node = iter_node->next;
        }
    }

    return is_keys_equal 
           ? (ErrorCounter)iter_node->elem.n_copies 
           : 0;
}

// static ---------------------------------------------------------------------

HOT static bool CompareKeysEqual(StringView* key1, StringView* key2) {
    ASSERT(key1 != NULL);
    ASSERT(key2 != NULL);

    size_t max_len = MAX(key1->len, key2->len);
    size_t min_len = MIN(key1->len, key2->len);

#if defined (OPT_ON)
    int first_check = FasterStrncmp(key1->str, key2->str, min_len);
#else
    int first_check = strncmp(key1->str, key2->str, min_len);
#endif // OPT_ON
    if (first_check != 0) {
        return false;
    }

    const char* iter = key1->len > key2->len ? key1->str : key2->str;
    for (Index i = min_len; i < max_len; i++) {
        if (iter[i] != '\0') {
            return false;
        }
    }

    return true;
}
