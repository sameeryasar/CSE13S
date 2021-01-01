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
// Declaring options for getopt, bit length helper function, and
// file protection struct
#define OPTIONS "vi:o:"

uint8_t bit_length(int code);

struct stat file_protection;

int main(int argc, char **argv) {
  bool decompression_stats = false, input = false, output = false;
  int to_decompress = STDIN_FILENO, decompressed = STDOUT_FILENO, option;
  char *input_name, *output_name;

  //
  // Parses through command line arguments and checks if
  // stats and/or specific input and output files are wanted
  while ((option = getopt(argc, argv, OPTIONS)) != -1) {
    switch (option) {
    case 'v':
      decompression_stats = true;
      break;
    case 'i':
      input_name = optarg;
      input = true;
      break;
    case 'o':
      output_name = optarg;
      output = true;
      break;
    }
  }

  //
  // Allocates memory for file header
  FileHeader *decompressed_fh = (FileHeader *)calloc(1, sizeof(FileHeader));
  if (!decompressed_fh) {
    printf("Failed to create file header\n");
    return -1;
  }
  //
  // If input was specified, opens said input file, otherwise will
  // take input from STDIN
  if (input) {
    to_decompress = open(input_name, O_RDONLY);
    if (to_decompress == -1) {
      printf("Could not open input file to be decode\n");
      close(to_decompress);
      free(decompressed_fh);
      return -1;
    }
  }

  //
  // If output was specified, opens or creats output file, otherwise will
  // write output to STDOUT
  if (output) {
    decompressed = open(output_name, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (decompressed == -1) {
      printf("Could not open decoded output file\n");
      close(to_decompress);
      close(decompressed);
      free(decompressed_fh);
      return -1;
    }
  }

  //
  // Reads the header of the compressed infile and checks
  // its protections
  read_header(to_decompress, decompressed_fh);
  fchmod(to_decompress, decompressed_fh->protection);

  //
  // If the compressed infile does not have the appropriate
  // magic identifier, will exit program
  if (decompressed_fh->magic != MAGIC) {
    printf("Incorrect magic in file header\n");
    close(to_decompress);
    close(decompressed);
    free(decompressed_fh);
    return -1;
  }

  //
  // LZ78 Decompression algorithm on infile
  WordTable *table = wt_create();
  uint8_t curr_sym = 0;
  uint16_t curr_code = 0;
  int next_code = START_CODE;

  while (
      read_pair(to_decompress, &curr_code, &curr_sym, bit_length(next_code))) {
    table[next_code] = word_append_sym(table[curr_code], curr_sym);
    buffer_word(decompressed, table[next_code]);
    next_code++;
    if (next_code == MAX_CODE) {
      wt_reset(table);
      next_code = START_CODE;
    }
  }

  flush_words(decompressed);

  //
  // Closes any files that were opened
  close(to_decompress);
  close(decompressed);

  //
  // Frees word table used by algorithm and file header struct
  wt_delete(table);
  free(decompressed_fh);

  //
  // If stats were specified, prints stats
  if (decompression_stats) {
    printf("Compressed file size: %lu bytes\n", total_bits / 8);
    printf("Uncompressed file size: %lu bytes\n", total_syms);
    printf(
        "Compression ratio: %0.2lu%%\n", (1 - ((total_bits / 8) / total_syms)));
  }

  printf("Decoded successfully\n");
  return 0;
}

uint8_t bit_length(int code) {
  return log2(code) + 1;
}
