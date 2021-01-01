#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "bv.h"
#include "sieve.h"

#define OPTIONS "spn:"
// 
// Declaring all prototypes of functions
//
bool fib_test(int n);
bool luc_test(int n);
bool mer_test(int n);
void reverse_str(char *str);
char* convert_base(char conversion[], int n, int base);
bool palindrome_test(char *str);
uint32_t bv_get_len(BitVector *v); 
//
// Main function accepts arguments from command line.
// such as -s to run sequence test, -p to run
// palindrome test, and -n to set the largest number
// to be considered (default = 1000)
//
int main(int argc, char **argv) {
  
  int option_val, n = 1000;
  bool sequence = false, palindrome = false;
  extern char *optarg;

  while ((option_val = getopt(argc, argv, OPTIONS)) != -1) {
    switch (option_val) {
    case 'n':
      if (optarg != NULL) {
        n = atoi(optarg);
      }
      break;
    case 's':
      sequence = true;
      break;
    case 'p':
      palindrome = true;
      break;
    }
  }
// 
// Creates a BitVector using n as its length,
// which is then sieved to produce prime numbers.
// Each prime number is printed and then checked if
// it is a mersenne, lucas, and/or fibonacci prime,
// which it will print
//
  if (sequence) {
    struct BitVector *seq = bv_create(n);
    sieve(seq);
    for (uint32_t i = 2; i < bv_get_len(seq); i++) {
      if (bv_get_bit(seq, i) == 1) {
        printf("%d: prime", i);
        if (mer_test(i)) {
          printf(", mersenne"); 
        }
        if (luc_test(i)) {
          printf(", lucas");
        }
        if (fib_test(i)) {
          printf(", fibonacci");
        }
      printf("\n");   
      }
    }
   
    bv_delete(seq);
  }
// 
// Creates a BitVector with n as its length, 
// which is then sieved to produce prime numbers.
// Each prime number is converted into binary, 
// base 9, decimal, and base 35 (Y + 10). If the
// conversion is a palindrome, the prime number
// and its conversion will be printed.
//
  if (palindrome) {
    struct BitVector *pal = bv_create(n);
    sieve(pal);
    char conversion[100];    

    printf("Base  2\n");
    printf("---- --\n");
    for (uint32_t i = 2; i < bv_get_len(pal); i++) {
      if (bv_get_bit(pal, i) == 1) {
        if (palindrome_test(convert_base(conversion, i, 2))) {
        printf("%d = %s\n", i, convert_base(conversion, i, 2));
        }
      }
    }

    printf("\n");
    printf("Base  9\n");
    printf("---- --\n");
    for (uint32_t i = 2; i < bv_get_len(pal); i++) {
      if (bv_get_bit(pal, i) == 1) {
        if (palindrome_test(convert_base(conversion, i, 9))) {
        printf("%d = %s\n", i, convert_base(conversion, i, 9));
        }
      }
    }
    
    printf("\n");
    printf("Base  10\n");
    printf("---- --\n");
    for (uint32_t i = 2; i < bv_get_len(pal); i++) {
      if (bv_get_bit(pal, i) == 1) {
        if (palindrome_test(convert_base(conversion, i, 10))) {
        printf("%d = %s\n", i, convert_base(conversion, i, 10));
        }
      }
    }
    
    printf("\n");
    printf("Base  35\n");
    printf("---- --\n");
    for (uint32_t i = 2; i < bv_get_len(pal); i++) {
      if (bv_get_bit(pal, i) == 1) {
        if (palindrome_test(convert_base(conversion, i, 35))) {
        printf("%d = %s\n", i, convert_base(conversion, i, 35));
        }
      }
    }
   
    bv_delete(pal);
  }
  
  return 0;
}
// 
// Function returns true if
// the input is part of the 
// fibonacci sequence
//
bool fib_test(int n) {
  int first_num = 0;
  int sec_num = 1;
  int third_num = first_num + sec_num;

  if (n == 0 || n == 1) {
    return true;
  } else {
    while (third_num < n) {
      first_num = sec_num;
      sec_num = third_num;
      third_num = first_num + sec_num;

      if (third_num == n) {
        return true;
      }
    }
  }
  return false;
}
// 
// Function returns true if the
// input is part of the Lucas
// sequence
//
bool luc_test(int n) {
  int first_num = 2;
  int sec_num = 1;
  int third_num = first_num + sec_num;

  if (n == 2 || n == 1 || n == 3) {
    return true;
  } else {
    while (third_num < n) {
      first_num = sec_num;
      sec_num = third_num;
      third_num = first_num + sec_num;

      if (third_num == n) {
        return true;
      }
    }
  }
  return false;
}
// 
// Function returns true if the
// input is part of the 
// mersenne sequence
//
bool mer_test(int n) {
  for (int i = 1;; ++i) {
    int mer_num = pow(2, i) - 1;
    if (mer_num > n) {
      return false;
    } else if (mer_num == n) {
      return true;
    }
  }
}
// 
// Reverses input string by
// switching around its characters
// from left and right indexes
//
void reverse_str(char *str) {
  int length = strlen(str);
  for (int i = 0; i < length / 2; i++) {
    char cur_char = str[i];
    str[i] = str[length - 1 - i];
    str[length - 1 - i] = cur_char;
  }
}
// 
// Converts decimal into any base,
// returning the conversion as a string
// array
// Logic derived from: Tutorialspoint's Number 
// System Conversion
//
char* convert_base(char conversion[], int n, int base) {
  int pos = 0, result = n, remainder; 
  char conv_char;
  while (result > 0) {
    remainder = result % base;
    if (remainder >= 0 && remainder <= 9) {
      conv_char = (remainder + '0');
    } else {
      conv_char = ((remainder - 9) +  '`');
    }
    conversion[pos++] = conv_char;
    result /= base; 
  }
  conversion[pos] = '\0';
  reverse_str(conversion);
  return conversion;
}
//
// Checks input string array to see
// if it is a palindrome by comparing 
// left and right indices as they get
// closer to the middle
// Logic derived from geeksforgeeks's 
// C program to check if given string is
// a palindrome
//
bool palindrome_test(char *str) {
  int left_index = 0;
  int right_index = strlen(str) - 1;
  
  while (right_index > left_index) {
    if (str[left_index++] != str[right_index--]) {
      return false;
    }
  }
  return true;
}
