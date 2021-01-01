#include "bf.h"
#include "bv.h"
#include "hash.h"
#include "ll.h"
#include "speck.h"
#include "utility.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

extern bool move_to_front;

int main() {
  HatterSpeak *test = hs_create("old", "hat");
  ListNode *mario = ll_node_create(test);
  if (!mario) {
    hs_delete(test);
  } else {
    ll_delete(mario);
  }
  return 0;
}
