#include "code.h"
#include "io.h"
#include "trie.h"
#include "word.h"
#include <fcntl.h>
#include <getopt.h>
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

//
// Declaring defintion for options, setting the struct for file
// protection, and declaring global variable for stats
#define OPTIONS "vi:o:"

uint8_t bit_length(int code);

struct stat file_protection;

uint64_t total_bits;
uint64_t total_syms;

int main(int argc, char **argv) {
  bool compression_stats = false, input = false, output = false;
  int to_compress = STDIN_FILENO, compressed = STDOUT_FILENO, option;
  char *input_name, *output_name;
  //
  // Iterating through command line arguments to see if
  // files are specified and stats should be printed
  while ((option = getopt(argc, argv, OPTIONS)) != -1) {
    switch (option) {
    case 'v':
      compression_stats = true;
      break;
    case 'i':
      input_name = optarg;
      input = true;
      //to_compress = open(optarg, O_RDONLY);
      break;
    case 'o':
      output_name = optarg;
      output = true;
      //compressed = open(optarg, O_WRONLY | O_CREAT | O_TRUNC, 0600);
      break;
    }
  }

  //
  // Allocates memory for file header
  FileHeader *compressed_fh = (FileHeader *)calloc(1, sizeof(FileHeader));
  if (!compressed_fh) {
    printf("Failed to create file header\n");
    close(to_compress);
    close(compressed);
    return -1;
  }

  //
  // If input file was specified, opens said file, otherwise
  // will take input from STDIN
  if (input) {
    to_compress = open(input_name, O_RDONLY);
    if (to_compress == -1) {
      printf("Could not open input file to be encoded\n");
      close(to_compress);
      free(compressed_fh);
      return -1;
    }
  }

  //
  // If output file was specified, creates or writes output to
  // said file, otherwise will print output to STDOUT
  if (output) {
    compressed = open(output_name, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (compressed == -1) {
      printf("Could not open encoded output file\n");
      close(to_compress);
      close(compressed);
      free(compressed_fh);
      return -1;
    }
  }

  //
  // Sets the header file with the appropriate magic
  // number and protection, then writes it to the file
  fstat(to_compress, &file_protection);
  compressed_fh->magic = MAGIC;
  compressed_fh->protection = file_protection.st_mode;
  write_header(compressed, compressed_fh);

  //
  // LZ78 Compression algorithm, provided by Asgn 7
  // lab manual
  TrieNode *root = trie_create();
  TrieNode *curr_node = root;
  if (!curr_node) {
    printf("Failed to set current node\n");
    close(to_compress);
    close(compressed);
    free(compressed_fh);
    trie_delete(root);
    return -1;
  }

  TrieNode *prev_node = NULL;

  uint8_t curr_sym = 0;
  uint8_t prev_sym = 0;

  int next_code = START_CODE;

  while (read_sym(to_compress, &curr_sym)) {
    TrieNode *next_node = trie_step(curr_node, curr_sym);
    if (next_node) {
      prev_node = curr_node;
      curr_node = next_node;
    } else {
      buffer_pair(compressed, curr_node->code, curr_sym, bit_length(next_code));
      curr_node->children[curr_sym] = trie_node_create(next_code);
      curr_node = root;
      next_code++;
    }

    if (next_code == MAX_CODE) {
      trie_reset(root);
      curr_node = root;
      next_code = START_CODE;
    }

    prev_sym = curr_sym;
  }

  if (curr_node != root) {
    buffer_pair(compressed, prev_node->code, prev_sym, bit_length(next_code));
    next_code = (next_code + 1) % MAX_CODE;
  }

  buffer_pair(compressed, STOP_CODE, 0, bit_length(next_code));
  flush_pairs(compressed);

  //
  // Closing any files opened and freeing trie and file header structs
  close(to_compress);
  close(compressed);

  trie_delete(root);
  free(compressed_fh);

  //
  // If stats were specified, print stats message
  if (compression_stats) {
    if (total_bits % 8 < 4) {
      printf("Compressed file size: %lu bytes\n", total_bits / 8);
    } else {
      printf("Compressed file size: %lu bytes\n", (total_bits / 8) + 1);
    }

    printf("Uncompressed file size: %lu bytes\n", total_syms);
    printf("Compression ratio: %.2f%%\n",
        100.0 * (1 - (((float)total_bits / 8) / (float)total_syms)));
  }

  printf("Encoded successfully\n");
  return 0;
}

//
// Helper function to get the bit length of a code
uint8_t bit_length(int code) {
  return log2(code) + 1;
}
