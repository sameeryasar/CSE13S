#include "stack.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
//
// Creates stack with allocated capacity and
// name specified
//
Stack *stack_create(int capacity, char name) {
  Stack *s = (Stack *)malloc(sizeof(Stack));
  if (!s) {
    return 0;
  }
  if (capacity < 1) {
    capacity = 1;
  }
  s->capacity = capacity;
  s->top = 0;
  s->items = (int *)malloc(sizeof(int) * capacity);
  s->name = name;
  if (!s->items) {
    return 0;
  }
  return s;
}
//
// Deletes stack by freeing stack and its
// items from memory
//
void stack_delete(Stack *s) {
  free(s->items);
  free(s);
  return;
}
//
// Pops the top item of a stack
//
int stack_pop(Stack *s) {
  if (!s) {
    return -1;
  }
  if (s->top > 0) {
    s->top -= 1;
    return s->items[s->top];
  } else {
    return -1;
  }
}
//
// Pushes an item specified onto
// given stack
//
void stack_push(Stack *s, int item) {
  if (!s) {
    return;
  }
  if (s->top == s->capacity) {
    s->capacity *= 2;
    s->items = (int *)realloc(s->items, s->capacity * sizeof(int));
  }
  if (s->items) {
    s->items[s->top] = item;
    s->top += 1;
  }
  return;
}
//
// Returns true if stack is empty by
// looking at the top of the stack
//
bool stack_empty(Stack *s) {
  return s->top == 0;
}
//
// Looks at the top of a stack to see
// what the top item is
//
int stack_peek(Stack *s) {
  if (s->top != 0) {
    return s->items[s->top - 1];
  } else {
    return 0;
  }
}
