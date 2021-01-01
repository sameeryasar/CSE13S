#include "bv.h"
#include <math.h>

uint32_t bv_get_len(BitVector *v);
void bv_set_bit(BitVector *v, uint32_t i);
//
// The Sieve of Eratothenes
// Sets bits in a BitVector representing prime numbers.
// Composite numbers are sieved out by clearing bits.
//
// v: The Bitvector to sieve.
//
// Provided by Asgn4 Lab Manual
//
void sieve(BitVector *v) {
  bv_set_all_bits(v);
  bv_clr_bit(v, 0);
  bv_clr_bit(v, 1);
  bv_set_bit(v, 2);
  for (uint32_t i = 2; i < sqrtl(bv_get_len(v)); i += 1) {
    // Prime means bit is set
    if (bv_get_bit(v, i)) {
      for (uint32_t k = 0; (k + i) * i <= bv_get_len(v); k += 1) {
        bv_clr_bit(v, (k + i) * i);
      }
    }
  }
  return;
}
