#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include "llist.h"
#include "hash.h"
#include "my_assert.h"
#include "my_typedefs.h"
#include "string_view.h"

typedef enum HashTableError {
  kHashTable_Success = 0,
  kHashTable_BadAlloc = 1,
  kHashTable_NoSuchElement = 2,
  kHashTable_BadInsert = 3,
  kHashTable_BadRemove = 4,
  kHashTable_BadLookUp = 5,
} HashTableError;

typedef struct HashTable {
  size_t n_buckets;
  LList* backets;
} HashTable;

HashTableError HashTable_Ctor(HashTable* hash_table);
void HashTable_Dtor(HashTable* hash_table);

HashTableError HashTable_InsertByKey(HashTable* hash_table, StringView* key);
HashTableError HashTable_RemoveByKey(HashTable* hash_table, StringView* key);
HashTableError HashTable_LookUpbyKey(HashTable* hash_table, StringView* key);

#endif  // HASH_TABLE_H_