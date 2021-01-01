#include "binary.h"
#include <inttypes.h>
#include <stdio.h>
//
// Binary Sort code derived from Asgn5 Lab Manual
//
void binary_sort(uint32_t arr[], int arr_length) {
  int moves = 0, compares = 0;

  for (int i = 1; i < arr_length; i++) {
    uint32_t value = arr[i];
    int left = 0;
    int right = i;

    while (left < right) {
      int mid = left + ((right - left) / 2);
      compares++;
      if (value >= arr[mid]) {
        left = mid + 1;
      } else {
        right = mid;
      }
    }
    for (int j = i; j > left; j--) {
      uint32_t temp = arr[j - 1];
      arr[j - 1] = arr[j];
      arr[j] = temp;
      moves += 3;
    }
  }
  printf("%d elements, %d moves, %d compares\n", arr_length, moves, compares);
  return;
}
