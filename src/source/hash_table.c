#include "hash_table.h"

#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "hash.h"
#include "hash_table_cfg.h"
#include "llist.h"
#include "my_assert.h"
#include "my_typedefs.h"

// static ---------------------------------------------------------------------

LList* Accessbucket(const HashTable* hash_table, HashValue hash_value);

// global ---------------------------------------------------------------------

HashTableError HashTable_Ctor(HashTable* hash_table) {
    ASSERT(hash_table != NULL);

    hash_table->n_buckets = kNumberOfBuckets;
    hash_table->buckets = (LList*)calloc(kNumberOfBuckets, sizeof(LList));
    if (hash_table->buckets == NULL) {
        return kHashTable_BadAlloc;
    }

    LListError error = kLList_Success;
    for (Index i = 0; i < kNumberOfBuckets; i++) {
        error = LList_Ctor(hash_table->buckets + i);
        if (error != kLList_Success) {
            return kHashTable_BadAlloc;
        }
    }

    return kHashTable_Success;
}

void HashTable_Dtor(HashTable* hash_table) {
    ASSERT(hash_table != NULL);

    for (Index i = 0; i < hash_table->n_buckets; i++) {
        LList_Dtor(hash_table->buckets + i);
    }

    free(hash_table->buckets);
    hash_table->n_buckets = 0;
}

void HashTable_Dump(const HashTable* hash_table) {
    ASSERT(hash_table != NULL);

#if defined(DEBUG_ON)
    fprintf(stderr, "Hash Table:\n");
    fprintf(stderr, "   number of buckets: %lu\n", hash_table->n_buckets);
    fprintf(stderr, "   backets: %p\n", hash_table->buckets);
    for (Index i = 0; i < hash_table->n_buckets; i++) {
        if (hash_table->buckets[i].is_valid) {
            fprintf(stderr, "       [%lu] valid %lu\n", i,
                    hash_table->buckets[i].n_items);
        } else {
            fprintf(stderr, "       [%lu] invalid %lu\n", i,
                    hash_table->buckets[i].n_items);
        }
    }
#endif  // DEBUG_ON
}

HashTableError HashTable_InsertByKey(HashTable* hash_table, StringView* key) {
    ASSERT(hash_table != NULL);
    ASSERT(key != NULL);
    ASSERT(key->str != NULL);

    HashValue hash_key = Hash((const uint8_t*)key->str, key->len, 0);
    LListError llist_error = kLList_Success;

    llist_error = LList_Insert(Accessbucket(hash_table, hash_key), key);

    switch (llist_error) {
        case kLList_Success:
            return kHashTable_Success;
        case kLList_InvalidLList:
            return kHashTable_InvalidTable;
        case kLList_CtorCantAlloc:
        case kLList_BadInsert:
        case kLList_NoElementToRemove:
        case kLList_LookUpCantFindElem:
        default:
            return kHashTable_BadInsert;
    }

    return kHashTable_Success;
}

HashTableError HashTable_RemoveByKey(HashTable* hash_table, StringView* key) {
    ASSERT(hash_table != NULL);
    ASSERT(key != NULL);
    ASSERT(key->str != NULL);

    HashValue hash_key = Hash((const uint8_t*)key->str, key->len, 0);
    LListError llist_error = kLList_Success;
    llist_error = LList_Remove(Accessbucket(hash_table, hash_key), key);

    switch (llist_error) {
        case kLList_Success:
            return kHashTable_Success;
        case kLList_NoElementToRemove:
            return kHashTable_NoSuchElement;
        case kLList_InvalidLList:
            return kHashTable_InvalidTable;
        case kLList_CtorCantAlloc:
        case kLList_LookUpCantFindElem:
        case kLList_BadInsert:
        default:
            return kHashTable_BadRemove;
    }
}

ErrorCounter HashTable_LookUpByKey(const HashTable* hash_table,
                                   StringView* key) {
    ASSERT(hash_table != NULL);
    ASSERT(key != NULL);
    ASSERT(key->str != NULL);

    HashValue hash_key = Hash((const uint8_t*)key->str, key->len, 0);

    return LList_LookUp(Accessbucket(hash_table, hash_key), key);
}

// static ---------------------------------------------------------------------

LList* Accessbucket(const HashTable* hash_table, HashValue hash_value) {
    ASSERT(hash_table != NULL);

    return hash_table->buckets + (hash_value % hash_table->n_buckets);
}