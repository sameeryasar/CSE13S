#include "bf.h"
#include "bv.h"
#include "speck.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//
// Constructor for a BloomFilter.
// size:	The number of entries in the BloomFilter.
//
BloomFilter *bf_create(uint32_t size) {
  BloomFilter *bf = (BloomFilter *)malloc(sizeof(BloomFilter));
  if (bf) {
    bf->primary[0] = 0xfc28ca6885711cf7;
    bf->primary[1] = 0x2841af568222f773;
    bf->secondary[0] = 0x85ae998311115ae3;
    bf->secondary[1] = 0xb6fac2ae33a40089;
    bf->tertiary[0] = 0xd37b01df0ae8f8d0;
    bf->tertiary[1] = 0x911d454886ca7cf7;
    bf->filter = bv_create(size);
    return bf;
  }
  return (BloomFilter *)NIL;
}

//
// Destructor for a BloomFilter.
// bf:	The BloomFilter.
//
void bf_delete(BloomFilter *bf) {
  bv_delete(bf->filter);
  free(bf);

  return;
}

//
// Inserts a new key into the BloomFilter.
// Indices to set bits are given by the hash functions.
//
// bf:	The BloomFilter.
// key:	The key to insert into the BloomFilter.
//
void bf_insert(BloomFilter *bf, char *key) {
  uint32_t p_index = hash(bf->primary, key) % bv_get_len(bf->filter);
  uint32_t s_index = hash(bf->secondary, key) % bv_get_len(bf->filter);
  uint32_t t_index = hash(bf->tertiary, key) % bv_get_len(bf->filter);

  bv_set_bit(bf->filter, p_index);
  bv_set_bit(bf->filter, s_index);
  bv_set_bit(bf->filter, t_index);

  return;
}

//
// Probes a BloomFilter to check if a key has been inserted.
//
// bf:	The BloomFilter.
// key:	The key in which to check membership.
//
bool bf_probe(BloomFilter *bf, char *key) {
  uint32_t p_index = hash(bf->primary, key) % bv_get_len(bf->filter);
  uint32_t s_index = hash(bf->secondary, key) % bv_get_len(bf->filter);
  uint32_t t_index = hash(bf->tertiary, key) % bv_get_len(bf->filter);

  if (bv_get_bit(bf->filter, p_index) && bv_get_bit(bf->filter, s_index)
      && bv_get_bit(bf->filter, t_index)) {
    return true;
  }
  return false;
}

//
// Iterates though the BloomFilter and adds up each
// set bit
//
// bf:	The BloomFilter.
//
uint32_t bf_count(BloomFilter *bf) {
  uint32_t set_bits = 0;

  for (uint32_t i = 0; i < bv_get_len(bf->filter); i++) {
    if (bv_get_bit(bf->filter, i)) {
      set_bits++;
    }
  }

  return set_bits;
}
