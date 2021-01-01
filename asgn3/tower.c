#include "stack.h"
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define OPTIONS "srn:"
//
// Declaring all protoype of functions
//
int recur(int n_discs, int start, int end);
void recur_print_move(int n_disc, int start, int end);
void stack_move_discs(
    struct Stack *start, struct Stack *end, char start_rod, char end_rod);
int stack_iterator(
    int n_discs, struct Stack *start, struct Stack *aux, struct Stack *end);
void stack_print_move(int disc, char start, char end);
double power(double base, unsigned short exponent);
const char *rods[] = { "A", "B", "C" };
//
// Function accepts arguments from command line
// and runs appropriate implementations
// as well as setting number of discs
//
int main(int argc, char **argv) {

  int option_val, n_discs = 5;
  bool recursion = false, stack = false;
  extern char *optarg;

  while ((option_val = getopt(argc, argv, OPTIONS)) != -1) {
    switch (option_val) {
    case 'n':
      if (optarg != NULL) {
        n_discs = atoi(optarg);
      }
      break;
    case 's':
      stack = true;
      break;
    case 'r':
      recursion = true;
      break;
    }
  }
  //
  // Checks if stack was given argument, then creates
  // each rod as a stack, runs the stack
  // implementation, then deletes stacks created
  //
  if (stack) {
    struct Stack *A = stack_create(n_discs, 'A');
    struct Stack *B = stack_create(n_discs, 'B');
    struct Stack *C = stack_create(n_discs, 'C');
    printf("================================\n");
    printf("----------   STACKS   ----------\n");
    printf("================================\n");
    printf("\nNumber of moves: %d\n\n", stack_iterator(n_discs, A, C, B));
    stack_delete(A);
    stack_delete(B);
    stack_delete(C);
  }
  //
  // Checks if recursion was given argument, then runs
  // recursive function
  //
  if (recursion) {
    printf("================================\n");
    printf("--------   RECURSION   ---------\n");
    printf("================================\n");
    printf("\nNumber of moves: %d\n\n", recur(n_discs, 1, 2));
  }

  return 0;
}
//
// Recursion function takes n_discs and start and
// end rods as inputs. Seperates ToH into smaller
// ToHs and exits with base case of 1 n_disc.
// Keeps track each time function makes a move
// and returns the number of moves at the end
// of the function.
//
int recur(int n_discs, int start, int end) {

  static int n_moves = 0;
  ++n_moves;

  if (n_discs == 1) {
    recur_print_move(n_discs, start, end);
  } else {
    int sub_hanoi = 6 - (start + end);
    recur(n_discs - 1, start, sub_hanoi);
    recur_print_move(n_discs, start, end);
    recur(n_discs - 1, sub_hanoi, end);
  }
  return n_moves;
}
//
// Helper function to print moves take in recur function
//
void recur_print_move(int n_discs, int start, int end) {
  printf("Move disk %d from peg %s to peg %s\n", n_discs, rods[start - 1],
      rods[end - 1]);
  return;
}
//
// Checks if movement between stacks for stack
// implementation is legal
//
void stack_move_discs(
    struct Stack *start, struct Stack *end, char start_rod, char end_rod) {
  int start_top_disc = stack_peek(start);
  int end_top_disc = stack_peek(end);

  if (stack_empty(start)) {
    stack_push(start, end_top_disc);
    stack_pop(end);
    stack_print_move(end_top_disc, end_rod, start_rod);
  } else if (stack_empty(end)) {
    stack_push(end, start_top_disc);
    stack_pop(start);
    stack_print_move(start_top_disc, start_rod, end_rod);
  } else if (start_top_disc > end_top_disc) {
    stack_push(start, end_top_disc);
    stack_pop(end);
    stack_print_move(end_top_disc, end_rod, start_rod);
  } else {
    stack_push(end, start_top_disc);
    stack_pop(start);
    stack_print_move(start_top_disc, start_rod, end_rod);
  }
  return;
}
//
// Iterates through the moves ToH takes, deciding where
// to move discs given the number of the turn.
// Returns the number of moves taken.
//
int stack_iterator(
    int n_discs, struct Stack *start, struct Stack *aux, struct Stack *end) {

  int i, n_moves, moves_taken = 0;
  char a = 'A', b = 'B', c = 'C';

  if (n_discs % 2 == 0) {
    char temp = b;
    b = c;
    c = temp;
  }
  n_moves = power(2, n_discs) - 1;

  for (i = n_discs; i >= 1; --i) {
    stack_push(start, i);
  }

  for (i = 1; i <= n_moves; ++i) {
    if (i % 3 == 1) {
      stack_move_discs(start, end, a, b);
    } else if (i % 3 == 2) {
      stack_move_discs(start, aux, a, c);
    } else if (i % 3 == 0) {
      stack_move_discs(aux, end, c, b);
    }
    ++moves_taken;
  }

  return moves_taken;
}
//
// Helper function to print movement of which disc
// from peg to peg
//
void stack_print_move(int disc, char start, char end) {
  printf("Move disk %d from peg %c to peg %c\n", disc, start, end);
  return;
}
//
// Power function to calculate total number of turns
// stack implementation should take
//
double power(double base, unsigned short exponent) {
  double product = 1;
  while (exponent > 0) {
    product *= base;
    --exponent;
  }
  return product;
}
