#include "ll.h"
#include "utility.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int seeks = 0;
int nodes_traveled = 0;

//
// Constructor for a ListNode.
//
// gs:	HatterSpeak struct containing oldspeak and its hatterspeak
// translation.
//
ListNode *ll_node_create(HatterSpeak *gs) {
  ListNode *n = (ListNode *)malloc(sizeof(ListNode));
  if (!n) {
    return NULL;
  }

  if (!gs) {
    free(n);
    return NULL;
  }

  n->gs = gs;
  n->next = NULL;

  return n;
}

//
// Destructor for a ListNode.
// n:	The ListNode to free.
//
void ll_node_delete(ListNode *n) {
  if (!n) {
    NULL;
  }
  free(n->gs->oldspeak);
  if (n->gs->hatterspeak) {
    free(n->gs->hatterspeak);
  }
  free(n->gs);
  free(n);

  return;
}

//
// Destructor for a linked list of ListNodes.
//
// head:	The head of the linked list.
void ll_delete(ListNode *head) {
  ListNode *temp = head;
  ListNode *next_node;

  while (temp != NULL) {
    next_node = temp->next;
    ll_node_delete(temp);
    temp = next_node;
  }

  return;
}

//
// Creates and inserts a ListNode into a linked list.
//
// head:	The head of the linked list to insert in.
// gs:		HatterSpeak struct.
ListNode *ll_insert(ListNode **head, HatterSpeak *gs) {
  if (ll_lookup(head, gs->oldspeak)) {
    hs_delete(gs);
    return *head;
  }

  ListNode *new = ll_node_create(gs);

  if (!new) {
    return NULL;
  }

  new->next = *head;
  *head = new;

  return new;
}

//
// Searches for a specific key in a linked list.
// Returns the ListNode if found and NULL otherwise.
//
// head:	The head of the linked list to search in.
// key:		The key to search for.
ListNode *ll_lookup(ListNode **head, char *key) {
  ListNode *temp = *head;
  ListNode *next = temp->next;

  seeks++;
  nodes_traveled++;

  if (!temp) {
    return NULL;
  }

  if (strcmp(temp->gs->oldspeak, key) == 0) {
    return temp;
  }

  while (next != NULL) {
    nodes_traveled++;
    if (strcmp(next->gs->oldspeak, key) == 0) {
      if (move_to_front) {
        //printf("moving to front\n");
        temp->next = next->next;
        next->next = *head;
        *head = next;
      }

      return next;
    }

    temp = next;
    next = next->next;
  }

  return NULL;
}

//
// Prints the oldspeak of a ListNode.
//
// head:	The head of the linked list to print.
void ll_print_old(ListNode *head) {
  if (!head) {
    return;
  }

  ListNode *temp = head;

  while (temp != NULL) {
    if (strcmp(temp->gs->oldspeak, "~") == 0) {
      temp = temp->next;
      continue;
    }
    printf("%s\n", temp->gs->oldspeak);
    temp = temp->next;
  }
}

//
// Prints the oldspeak and its hatterspeak
// translation of a ListNode.
//
// head:	The head of the linked list to print.
void ll_print_translate(ListNode *head) {
  if (!head) {
    return;
  }

  ListNode *temp = head;

  while (temp != NULL) {
    if (strcmp(temp->gs->oldspeak, "~") == 0) {
      temp = temp->next;
      continue;
    }
    printf("%s -> %s\n", temp->gs->oldspeak, temp->gs->hatterspeak);
    temp = temp->next;
  }
}

//
// Returns the number of nodes in a
// linked list.
//
// head:	The head of the linked list to measure.
int ll_length(ListNode *head) {
  ListNode *temp = head;
  int n_nodes = 0;

  while (temp != NULL) {
    n_nodes += 1;
    temp = temp->next;
  }

  return n_nodes;
}

//
// Void function to avoid dead stores.
//
// head:	The head of the linked list.
void ll_void(ListNode *head) {
  return;
}
