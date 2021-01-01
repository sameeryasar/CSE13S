#include "bv.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
// 
// Creates a BitVector with a corresponding length
// and specific item size
//
BitVector *bv_create(uint32_t bit_len) {
  BitVector *v = (BitVector *)malloc(sizeof(BitVector));
  if (!v) {
    return 0;
  }
  v->length = bit_len;
  v->vector = (uint8_t *)calloc(bit_len / 8 + 1, sizeof(uint8_t));

  return v;
}
// 
// Frees memory from the heap 
// allocated when creating
// a BitVector
//
void bv_delete(BitVector *v) {
  free(v->vector);
  free(v);

  return;
}
// 
// Returns the length of a BitVector
//
uint32_t bv_get_len(BitVector *v) {
  return v->length;
}
// 
// Sets a specific bit in input BitVector
//
void bv_set_bit(BitVector *v, uint32_t i) {
  i %= v->length;
  uint8_t byte = v->vector[i / 8];
  uint8_t position = i % 8;
  v->vector[i / 8] = byte | (1 << position);

  return;
}
// 
// Clears a specific bit in input BitVector
//
void bv_clr_bit(BitVector *v, uint32_t i) {
  i %= v->length;
  uint8_t byte = v->vector[i / 8];
  uint8_t position = i % 8;
  uint8_t mask = ~(1 << position);
  v->vector[i / 8] = byte & mask;

  return;
}
// 
// Returns a specific bit given input BitVector
// and index
//  
uint8_t bv_get_bit(BitVector *v, uint32_t i) {
  uint8_t position = i % 8;

  return ((v->vector[i / 8] & (1 << position)) >> position);
}
// 
// Uses to bv_set_bit to iterate through
// BitVector, setting each bit
//
void bv_set_all_bits(BitVector *v) {
  for (uint32_t i = 0; i < v->length; ++i) {
    bv_set_bit(v, i);
  }

  return;
}
