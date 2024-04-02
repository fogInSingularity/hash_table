#include "hash_table.h"

#include "hash.h"
#include "hash_table_cfg.h"
#include "llist.h"
#include "my_assert.h"

// static ---------------------------------------------------------------------

LList* AccessBacket(const HashTable* hash_table, HashValue hash_value);

// global ---------------------------------------------------------------------

HashTableError HashTable_Ctor(HashTable* hash_table) {
  ASSERT(hash_table != NULL);

  hash_table->n_buckets = kNumberOfBuckets;
  hash_table->backets = (LList*)calloc(kNumberOfBuckets, sizeof(LList));
  if (hash_table->backets == NULL) {
    return kHashTable_BadAlloc;
  }

  LListError error = kLList_Success;
  for (Index i = 0; i < kNumberOfBuckets; i++) {
    error = LList_Ctor(hash_table->backets + i);
    if (error != kLList_Success) {
      return kHashTable_BadAlloc;
    }
  }

  return kHashTable_Success;
}

void HashTable_Dtor(HashTable* hash_table) {
  ASSERT(hash_table != NULL);

  for (Index i = 0; i < hash_table->n_buckets; i++) {
    LList_Dtor(hash_table->backets + i);
  }

  free(hash_table->backets);
  hash_table->n_buckets = 0;
}

HashTableError HashTable_InsertByKey(HashTable* hash_table, StringView* key) {
  ASSERT(hash_table != NULL);
  ASSERT(key != NULL);
  ASSERT(key->str != NULL);

  HashValue hash_key = Hash((const uint8_t*)key->str, key->len, 0);
  LListError llist_error = kLList_Success;
  llist_error = LList_Insert(AccessBacket(hash_table, hash_key), key);
  if (llist_error != kLList_Success) {
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
  llist_error = LList_Remove(AccessBacket(hash_table, hash_key), key);
  if (llist_error != kLList_Success) {
    return kHashTable_BadRemove;
  }

  return kHashTable_Success;
}

HashTableError HashTable_LookUpbyKey(HashTable* hash_table, StringView* key) {
  ASSERT(hash_table != NULL);
  ASSERT(key != NULL);
  ASSERT(key->str != NULL);

  HashValue hash_key = Hash((const uint8_t*)key->str, key->len, 0);
  LListError llist_error = kLList_Success;
  llist_error = LList_LookUp(AccessBacket(hash_table, hash_key), key);
  if (llist_error != kLList_Success) {
    return kHashTable_BadLookUp;
  }

  return kHashTable_Success;
}

// static ---------------------------------------------------------------------

LList* AccessBacket(const HashTable* hash_table, HashValue hash_value) {
  ASSERT(hash_table != NULL);

  return hash_table->backets + (hash_value % hash_table->n_buckets);
}