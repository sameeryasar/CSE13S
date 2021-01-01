#ifndef __QUICK_H__
#define __QUICK_H__

#include <inttypes.h>
#include <stdio.h>
//
// Declaration of quick sort and partition
// functions, as well as extern variables
// for quick sort
//
extern int q_moves, q_compares;

int partition(uint32_t arr[], int left, int right);

void quick_sort(uint32_t arr[], int left, int right);

#endif
