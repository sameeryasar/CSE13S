#include "trie.h"
#include "code.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

//
// Constructor for a TrieNode.
//
// Code:	Code of the constructed TrieNode.
// returns:	Pointer to a TrieNode that has been allocated memory.
//
TrieNode *trie_node_create(uint16_t code) {
  TrieNode *n = (TrieNode *)malloc(sizeof(TrieNode));
  if (!n) {
    return NULL;
  }

  n->code = code;

  for (int i = 0; i < 256; i++) {
    n->children[i] = NULL;
  }

  return n;
}

//
// Destructor for a TrieNode.
//
// n:		TrieNode to free allocated memory for.
// returns:	Void.
void trie_node_delete(TrieNode *n) {
  if (!n) {
    return;
  }

  free(n);

  return;
}

//
// Initializes a Trie:	a root TrieNode with the code EMPTY_CODE.
//
// returns:	Pointer to the root of a Trie.
//
TrieNode *trie_create(void) {
  TrieNode *node = trie_node_create(EMPTY_CODE);

  return node;
}

//
// Resets a Trie to just the root TrieNode.
//
// root:	Root of the Trie to reset.
// returns:	Void.
void trie_reset(TrieNode *root) {
  if (!root) {
    return;
  }

  for (int i = 0; i < 256; i++) {
    if (root->children[i] != NULL) {
      //printf("%c found a child in reset\n", i);
      trie_delete(root->children[i]);
      root->children[i] = NULL;
    }
  }

  return;
}

//
// Deletes a sub-Trie starting from the sub-Trie's root.
//
// n:		Root of the sub-Trie to delete.
// returns:	Void.
void trie_delete(TrieNode *n) {
  if (!n) {
    return;
  }

  for (int i = 0; i < 256; i++) {
    if (n->children[i] != NULL) {
      //printf("%c found a child in delete\n", i);
      trie_delete(n->children[i]);
      n->children[i] = NULL;
    }
  }

  trie_node_delete(n);

  return;
}

//
// Returns a pointer to the child TrieNode representing the symbol sym.
// If the symbol doesn't exist, NULL is returned.
//
// n:		TrieNode to step from.
// sym:		Symbol to check for.
// returns:	Pointer to the TrieNode representing the symbol.
TrieNode *trie_step(TrieNode *n, uint8_t sym) {
  return n->children[(int)sym];
}

//
// Helper function to test if trie node is properly working.
//
// Prints the characters stored in the node's children array.
void tn_print(TrieNode *n) {
  if (!n) {
    return;
  }

  for (int i = 0; i < 256; i++) {
    if (n->children[i] != NULL) {
      printf("%c--", i);
    }
  }
  printf("\n");
}
