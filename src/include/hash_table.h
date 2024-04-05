#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include "llist.h"
#include "string_view.h"

typedef enum HashTableError {
    kHashTable_Success = 0,
    kHashTable_InvalidTable = 1,
    kHashTable_BadAlloc = 2,
    kHashTable_NoSuchElement = 3,
    kHashTable_BadInsert = 4,
    kHashTable_BadRemove = 5,
    kHashTable_BadLookUp = 6,
} HashTableError;

typedef struct HashTable {
    size_t n_buckets;
    LList* buckets;
} HashTable;

HashTableError HashTable_Ctor(HashTable* hash_table);
void HashTable_Dtor(HashTable* hash_table);
void HashTable_Dump(const HashTable* hash_table);

HashTableError HashTable_InsertByKey(HashTable* hash_table, StringView* key);
HashTableError HashTable_RemoveByKey(HashTable* hash_table, StringView* key);
ErrorCounter HashTable_LookUpByKey(const HashTable* hash_table,
                                   StringView* key);

#endif  // HASH_TABLE_H_