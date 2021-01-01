#include "bubble.h"
#include <inttypes.h>
#include <stdio.h>
//
// Bubble Sort derived from Asgn5 Lab Manual
//
void bubble_sort(uint32_t arr[], int arr_length) {
  int moves = 0, compares = 0;

  for (int i = 0; i < arr_length - 1; i++) {
    int j = arr_length - 1;
    while (j > i) {
      compares++;
      if (arr[j] < arr[j - 1]) {
        uint32_t temp = arr[j];
        arr[j] = arr[j - 1];
        arr[j - 1] = temp;
        moves += 3;
      }
      j--;
    }
  }
  printf("%d elements, %d moves, %d compares\n", arr_length, moves, compares);
  return;
}
