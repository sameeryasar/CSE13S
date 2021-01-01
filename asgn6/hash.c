#include "hash.h"
#include "ll.h"
#include "speck.h"
#include <inttypes.h>
#include <stdlib.h>

int seeks;

//
// Constructor for a HashTable.
//
// length:	Length of the HashTable.
// salt:	Salt for the HashTable.
//
HashTable *ht_create(uint32_t length) {
  HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
  if (ht) {
    ht->salt[0] = 0x85ae998311115ae3;
    ht->salt[1] = 0xb6fac2ae33a40089;
    ht->length = length;
    ht->heads = (ListNode **)calloc(length, sizeof(ListNode *));
    if (!ht->heads) {
      free(ht);
      return NULL;
    }

    return ht;
  }

  return (HashTable *)NIL;
}
//
// Destructor for a HashTable.
//
// ht:	The HashTable.
//
void ht_delete(HashTable *ht) {
  if (!ht) {
    return;
  }

  for (uint32_t i = 0; i < ht->length; i++) {
    //printf("\ndeleting a head\n");
    ll_delete(ht->heads[i]);
  }

  free(ht->heads);
  free(ht);
  return;
}

//
// Returns number of entries in hash table
//
// h:	The HashTable.
//
uint32_t ht_count(HashTable *ht) {
  if (!ht) {
    return 0;
  }

  uint32_t n_heads = 0;

  for (uint32_t i = 0; i < ht->length; i++) {
    ListNode *temp = ht->heads[i];

    if (temp != NULL) {
      n_heads += 1;
    }
  }

  return n_heads;
}

//
// Searches a HashTable for a key.
// Returns the ListNode if found and returns NULL otherwise.
// This should call the ll_loopup() function.
//
// ht:	The HashTable.
// key:	The key to search for
//
ListNode *ht_lookup(HashTable *ht, char *key) {
  if (!ht) {
    return NULL;
  }

  uint32_t index = hash(ht->salt, key) % ht->length;

  if (!ht->heads[index]) {
    return NULL;
  }

  ListNode *new = ll_lookup(&ht->heads[index], key);

  return new;
}

//
// First creates a new ListNode from HatterSpeak.
// The created ListNode is then inserted into a HashTable.
// This should call the ll_insert() function.
//
// ht:	The HashTable.
// gs:	The HatterSpeak to add to the HashTable.
//
void ht_insert(HashTable *ht, HatterSpeak *gs) {
  uint32_t index = hash(ht->salt, gs->oldspeak) % ht->length;

  if (!ht->heads[index]) {
    ht->heads[index] = ll_node_create(gs);
    seeks++;
  } else {
    ht->heads[index] = ll_insert(&ht->heads[index], gs);
  }

  return;
}

//
// Counts the length of each linked list
// inserted in the HashTable.
//
// ht:	The HashTable.
//
int ht_total_length(HashTable *ht) {
  if (!ht) {
    return 0;
  }

  int total_length = 0;

  for (uint32_t i = 0; i < ht->length; i++) {
    total_length += ll_length(ht->heads[i]);
  }

  return total_length;
}
