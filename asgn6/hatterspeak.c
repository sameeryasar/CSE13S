#include "bf.h"
#include "bv.h"
#include "hash.h"
#include "ll.h"
#include "parser.h"
#include "speck.h"
#include "utility.h"
#include <ctype.h>
#include <getopt.h>
#include <inttypes.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//
// Defining command line arguments for getopt
// and regex input
#define OPTIONS "mbsh:f:"
#define REGEX "[A-Za-z0-9_]+(('|-)[a-zA-Z0-9_]*)*"
//
// Declaring extern variables
bool move_to_front;
int seeks;
int nodes_traveled;
//
// Main function that reads in arguments and checks if input stream is
// a oldspeak, a translatable oldspeak, or hatterspeak using BloomFilters
// and HashTables. Either prints statistics or appropriate letter message.
int main(int argc, char **argv) {
  uint32_t ht_length = 10000, bf_length = 1048576;
  bool stats = false;
  bool m = false, b = false;
  char option;
  //
  // Parses through command line arguments and sets specific
  // variables to perform certain functions.
  while ((option = getopt(argc, argv, OPTIONS)) != -1) {
    switch (option) {
    case 's':
      stats = true;
      break;
    case 'h':
      if (atoi(optarg) < 1) {
        printf("Hash table length must be 1 or greater.\n");
        return 0;
      } else {
        ht_length = (uint32_t)atoi(optarg);
      }
      break;
    case 'f':
      if (atoi(optarg) < 1) {
        printf("Bloom filter length must be 1 or greater.\n");
      } else {
        bf_length = (uint32_t)atoi(optarg);
      }
      break;
    case 'm':
      move_to_front = true;
      m = true;
      break;
    case 'b':
      move_to_front = false;
      b = true;
      break;
    }
  }
  //
  // If both m and b were specifiec, return an error message.
  if (m && b) {
    printf("Cannot specify both -m and -b; only use one.\n");
    return 0;
  }
  //
  // Create a BloomFilter for each oldspeak and hatterspeak, as well
  // as a HashTable for hatterspeak pairs.
  BloomFilter *bf = bf_create(bf_length);
  BloomFilter *hatterspeaks = bf_create(bf_length);
  HashTable *ht = ht_create(ht_length);

  //
  // Open up oldspeak.txt and read through as you add it into the
  // BloomFilter and HashTable.
  FILE *oldspeak_file = fopen("oldspeak.txt", "r");
  if (oldspeak_file == NULL) {
    return 0;
  }

  char old_buffer[100];
  while (fscanf(oldspeak_file, "%s\n", old_buffer) != EOF) {
    bf_insert(bf, old_buffer);
    HatterSpeak *old_word = hs_create(old_buffer, NULL);
    ht_insert(ht, old_word);
  }

  fclose(oldspeak_file);
  //
  // Open up hatterspeak.txt and read through as you add keys into the
  // BloomFilter, hatterspeaks into the hatterspeak BloomFilter, and the
  // pairs into the HashTable.
  FILE *hatterspeak_file = fopen("hatterspeak.txt", "r");
  if (hatterspeak_file == NULL) {
    return 0;
  }

  char key_buffer[250];
  char hatter_buffer[250];
  while (
      fscanf(hatterspeak_file, "%s  %s\n", key_buffer, hatter_buffer) != EOF) {
    HatterSpeak *hatter_word = hs_create(key_buffer, hatter_buffer);
    ht_insert(ht, hatter_word);
    bf_insert(bf, key_buffer);
    bf_insert(hatterspeaks, hatter_buffer);
  }

  fclose(hatterspeak_file);
  //
  // Check if regcom properly compiles.
  regex_t word_regex;

  if (regcomp(&word_regex, REGEX, REG_EXTENDED) != 0) {
    printf("ERROR\n");
    return 0;
  }
  //
  // Create linked lists for bad words and revisable words.
  HatterSpeak *place_holder = hs_create("~", NULL);
  HatterSpeak *place_holder2 = hs_create("~", NULL);

  ListNode *bad = ll_node_create(place_holder);
  ListNode *revised = ll_node_create(place_holder2);

  char *new_word;

  //
  // Loop through the input stream, configured by tolower and REGEX, and
  // determine if it is a fine word, bad word, a false positive, or a
  // translatable word. Add it into their respecitive lists.
  while ((new_word = next_word(stdin, &word_regex)) != NULL) {
    for (int i = 0; i < strlen(new_word); i++) {
      new_word[i] = tolower(new_word[i]);
    }
    //printf("%s\n", new_word);
    bool in_filter = bf_probe(bf, new_word);
    if (in_filter) {
      //printf("\n\nin filter\n\n");
      if (bf_probe(hatterspeaks, new_word)) {
        continue;
      }
      ListNode *check_ht = ht_lookup(ht, new_word);
      if (!check_ht) {
        continue;
      }
      if (check_ht->gs->hatterspeak) {
        //printf("fine word\n");
        HatterSpeak *translate = hs_create(new_word, check_ht->gs->hatterspeak);
        ListNode *translated = ll_insert(&revised, translate);
        ll_void(translated);
      } else if (!check_ht->gs->hatterspeak) {
        //printf("bad word\n");
        if (bf_probe(hatterspeaks, new_word)) {
          continue;
        }
        HatterSpeak *old = hs_create(new_word, NULL);
        ListNode *old_word = ll_insert(&bad, old);
        ll_void(old_word);
      }
    }
  }
  //printf("Clearing words and reg\n");
  clear_words();
  regfree(&word_regex);
  //
  // If stats was specified, print seeks, average seek length,
  // average linked list, hash table load, and bloom filter load.
  // Free all structs afterwards, and exit program.
  if (stats) {
    //printf("Stats\n");
    printf("Seeks: %d\n", seeks);
    //printf("Nodes: %d\n", nodes_traveled);
    printf("Average seek length: %f\n", ((float)nodes_traveled / (float)seeks));
    printf("Average Linked List Length: %f\n",
        ((float)ht_total_length(ht) / (float)ht->length));
    printf("Hash table Load: %f%%\n",
        ((float)ht_count(ht) / (float)ht->length) * 100);
    printf("Bloom filter Load: %f%%\n",
        ((float)bf_count(bf) / (float)bv_get_len(bf->filter)) * 100);

    ll_delete(bad);
    ll_delete(revised);
    bf_delete(hatterspeaks);
    bf_delete(bf);
    ht_delete(ht);

    return 0;
  }

  //
  // If stats was not specified, print appropriate letter depending on
  // number of items in bad and revised linked lists and their possible errors
  // and traslations. Free all structs afterwards, and exit program.
  if (ll_length(revised) > 1 && ll_length(bad) > 1) {
    printf("\nDear Comrade,\n\n");
    printf(
        "You have chosen to use words that the queen has decreed oldspeak.\n");
    printf("Due to your infraction you will be sent to the dungeon where you "
           "will\n");
    printf("   be taught hatterspeak.\n");
    printf("\nYour errors:\n\n");
    ll_print_old(bad);
    printf("\nAppropriate hatterspeak translations.\n\n");
    ll_print_translate(revised);
  } else if (ll_length(revised) > 1) {
    printf("\nDear Wonderlander,\n\n");
    printf(
        "The decree for hatterspeak finds your message lacking. Some of the\n");
    printf("   words that you used are not hatterspeak.\n");
    printf(
        "The list shows how to turn the oldspeak words into hatterspeak.\n\n");
    ll_print_translate(revised);
  } else if (ll_length(bad) > 1) {
    printf("\nDear Wonderlander,\n\n");
    printf("You have chose to use words that queen has decreed oldspeak.\n");
    printf("Due to your infraction you will be sent to the dungeon where you "
           "will\n");
    printf("    be taught hatterspeak.\n");
    printf("\nYour errors:\n\n");
    ll_print_old(bad);
  }

  ll_delete(bad);
  ll_delete(revised);
  bf_delete(hatterspeaks);
  bf_delete(bf);
  ht_delete(ht);

  return 0;
}
