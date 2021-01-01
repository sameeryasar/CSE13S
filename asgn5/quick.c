#include "quick.h"
#include <inttypes.h>
#include <stdio.h>
//
// Quicksort derived from Asgn5 Lab Manual
// (Includes extern variables to call on in
// sorting.c)
//
int q_moves = 0, q_compares = 0;

int partition(uint32_t arr[], int left, int right) {
  uint32_t pivot = arr[left];
  int lo = left + 1;
  int hi = right;

  while (1) {
    q_compares++;
    while (lo <= hi && arr[hi] >= pivot) {
      hi--;
      q_compares++;
    }
    q_compares++;
    while (lo <= hi && arr[lo] <= pivot) {
      lo++;
      q_compares++;
    }
    if (lo <= hi) {
      uint32_t temp = arr[lo];
      arr[lo] = arr[hi];
      arr[hi] = temp;
      q_moves += 3;
    } else {
      break;
    }
  }
  uint32_t temp = arr[left];
  arr[left] = arr[hi];
  arr[hi] = temp;
  q_moves += 3;

  return hi;
}

void quick_sort(uint32_t arr[], int left, int right) {
  if (left < right) {
    int index = partition(arr, left, right);
    quick_sort(arr, left, index - 1);
    quick_sort(arr, index + 1, right);
  }
  return;
}
