#include "word.h"
#include "code.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// Constructor for a word.
//
// syms:	Array of symbols a Word represents.
// len:		Length of the array of symbols.
// returns:	Pointer to a Word that has been allocated memory.
Word *word_create(uint8_t *syms, uint64_t len) {
  Word *w = (Word *)malloc(sizeof(Word));
  if (!w) {
    return NULL;
  }

  if (!syms) {
    //printf("empty code\n");
    w->syms = 0;
    w->len = 0;
    return w;
  }

  w->syms = (uint8_t *)malloc(sizeof(uint8_t) * len);
  if (!w->syms) {
    free(w);
    return NULL;
  }

  w->len = len;

  for (uint64_t i = 0; i < w->len; i++) {
    w->syms[i] = syms[i];
  }

  return w;
}

//
// Constructs a new Word from the specified Word appended with a symbol.
// The Word specified to append to may be empty.
// If the above is the case, the new Word should contain only the symbol.
//
// w:		Word to append to.
// sym:		Symbol to append.
// returns:	New Word which represents the result of appending.
Word *word_append_sym(Word *w, uint8_t sym) {
  if (!w) {
    //printf("failed word appen\n");
    return NULL;
  }

  Word *x = (Word *)malloc(sizeof(Word));
  if (!x) {
    return NULL;
  }

  x->len = w->len + 1;

  x->syms = (uint8_t *)malloc(sizeof(uint8_t) * x->len);
  if (!x->syms) {
    free(x);
    return NULL;
  }

  memcpy(x->syms, w->syms, w->len);
  if (!x->syms) {
    free(x->syms);
    free(x);
    return NULL;
  }

  x->syms[x->len - 1] = sym;

  return x;
}

//
// Destructor for a Word.
// w: 		Word to free memory for.
// returns:	Void.
void word_delete(Word *w) {
  if (!w) {
    return;
  }

  free(w->syms);
  free(w);

  return;
}

//
// Creates a new WordTable, which is an array of Words.
// A WordTable has a pre-defined size of MAX_CODE (UINT16_MAX - 1).
// This is because codes are 16-bit integers.
// A WordTable is initialized with a single Word at index EMPTY_CODE.
// This Word represents the empty word, a string of length of zero.
//
// returns:	Initialized WordTable.
WordTable *wt_create(void) {
  WordTable *wt = (WordTable *)calloc(MAX_CODE, sizeof(Word));
  if (!wt) {
    return NULL;
  }

  wt[EMPTY_CODE] = word_create(NULL, 0);

  return wt;
}

//
// Resets a WordTable to having just the empty Word.
//
// wt:		WordTable to reset.
// returns: 	Void.
void wt_reset(WordTable *wt) {
  if (!wt) {
    return;
  }

  for (uint16_t i = START_CODE; i < MAX_CODE; i++) {
    word_delete(wt[i]);
    wt[i] = NULL;
  }

  return;
}

//
// Deletes an entire WordTable.
// All Words in the WordTable must be deleted as well.
//
// wt:		WordTable to free memory.
// returns:	Void.
void wt_delete(WordTable *wt) {
  if (!wt) {
    return;
  }

  for (uint16_t i = 0; i < MAX_CODE; i++) {
    if (wt[i] != NULL) {
      word_delete(wt[i]);
      wt[i] = NULL;
    }
  }

  free(wt);
  return;
}
