#include "io.h"
#include "word.h"
#include <fcntl.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

uint64_t total_syms;
uint64_t total_bits;

static uint8_t sym_buff[4096];
static int sym_cnt = 0;

static uint8_t bit_buff[4096];
static int bit_cnt = 0;

//
// Wrapper for the read() syscall.
// Loops to read the specified number of bytes, or until input is exhausted.
// Returns the number of bytes read.
//
// infile:  File descriptor of the input file to read from.
// buf:     Buffer to store read bytes into.
// to_read: Number of bytes to read.
// returns: Number of bytes read.
int read_bytes(int infile, uint8_t *buf, int to_read) {
  int bytes_read = 1;
  int cnt = to_read;
  int total = 0;

  while ((total != to_read) && (bytes_read != 0)) {
    bytes_read = read(infile, buf + total, cnt);
    total += bytes_read;
    cnt -= bytes_read;
  }

  return total;
}

//
// Wrapper for the write() syscall.
// Loops to write the specified number of bytes, or until nothing is written.
// Returns the number of bytes written.
//
// outfile:   File descriptor of the output file to write to.
// buf:       Buffer that stores the bytes to write out.
// to_write:  Number of bytes to write.
// returns:   Number of bytes written.
int write_bytes(int outfile, uint8_t *buf, int to_write) {
  int bytes_written = 1;
  int cnt = to_write;
  int total = 0;

  while ((total != to_write) && (bytes_written != 0)) {
    bytes_written = write(outfile, buf + total, cnt);
    total += bytes_written;
    cnt -= bytes_written;
  }

  return total;
}

//
// Reads in a FileHeader from the input file.
//
// infile:  File descriptor of input file to read header from.
// header:  Pointer to memory where the bytes of the read header should go.
// returns: Void.
void read_header(int infile, FileHeader *header) {
  read_bytes(infile, (uint8_t *)header, sizeof(header));
}

//
// Writes a FileHeader to the output file.
//
// outfile: File descriptor of output file to write header to.
// header:  Pointer to the header to write out.
// returns: Void.
void write_header(int outfile, FileHeader *header) {
  int header_bytes
      = write_bytes(outfile, (uint8_t *)header, sizeof(header)) * 8;
  total_bits += header_bytes;
}

//
// "Reads" a symbol from the input file.
// The "read" symbol is placed into the pointer to sym (pass by reference).
// In reality, a block of symbols is read into a buffer.
// An index keeps track of the currently read symbol in the buffer.
// Once all symbols are processed, another block is read.
// If less than a block is read, the end of the buffer is updated.
// Returns true if there are symbols to be read, false otherwise.
//
// infile:  File descriptor of input file to read symbols from.
// sym:     Pointer to memory which stores the read symbol.
// returns: True if there are symbols to be read, false otherwise.
bool read_sym(int infile, uint8_t *sym) {
  static int end = 0;
  if (!sym_cnt) {
    //printf("first sym\n");
    end = read_bytes(infile, sym_buff, BLOCK);
  }

  *sym = sym_buff[sym_cnt++];
  total_syms++;

  if (sym_cnt == BLOCK) {
    //printf("End of sym\n");
    sym_cnt = 0;
  }

  if (end == BLOCK) {
    //printf("End is block\n");
    return true;
  } else {
    if (sym_cnt == end + 1) {
      //printf("No more code to read\n");
      return false;
    } else {
      //printf("Sym index is not end + 1\n");
      return true;
    }
  }
}

//
// Buffers a pair. A pair is comprised of a symbol and an index.
// The bits of the symbol are buffered first, starting from the LSB.
// The bits of the index are buffered next, also starting from the LSB.
// bit_len bits of the index are buffered to provide a minimal representation.
// The buffer is written out whenever it is filled.
//
// outfile: File descriptor of the output file to write to.
// sym:     Symbol of the pair to buffer.
// index:   Index of the pair to buffer.
// bit_len: Number of bits of the index to buffer.
// returns: Void.
void buffer_pair(int outfile, uint16_t code, uint8_t sym, uint8_t bit_len) {
  total_bits += 8 + bit_len;
  //printf("bit_len %d\n", bit_len);
  for (uint8_t i = 0; i < bit_len; i++) {
    if (code & (1 << (i % 16))) {
      bit_buff[bit_cnt / 8] |= (1 << (bit_cnt % 8));
    } else {
      bit_buff[bit_cnt / 8] &= ~(1 << (bit_cnt % 8));
    }

    bit_cnt++;
    //printf("bit: %d\n", bit_cnt);
    if (bit_cnt / 8 == 4096) {
      //printf("writing 1\n");
      write_bytes(outfile, bit_buff, 4096);
      bit_cnt = 0;
    }
  }

  for (uint8_t i = 0; i < 8; i++) {
    if (sym & (1 << (i % 8))) {
      bit_buff[bit_cnt / 8] |= (1 << (bit_cnt % 8));
    } else {
      bit_buff[bit_cnt / 8] &= ~(1 << (bit_cnt % 8));
    }

    bit_cnt++;
    //printf("bit: %d\n", bit_cnt);
    if (bit_cnt / 8 == 4096) {
      //printf("writing 2\n");
      write_bytes(outfile, bit_buff, 4096);
      bit_cnt = 0;
    }
  }
}

//
// Writes out any remaining pairs of symbols and indexes to the output file.
//
// outfile: File descriptor of the output file to write to.
// returns: Void.
void flush_pairs(int outfile) {
  if (bit_cnt != 0) {
    //printf("bit index is not 0\n");
    int bytes;
    if (!(bit_cnt % 8)) {
      bytes = bit_cnt / 8;
    } else {
      bytes = (bit_cnt / 8) + 1;
    }
    //printf("writing 3\n");
    write_bytes(outfile, bit_buff, bytes);
  }
}

//
// "Reads" a pair (symbol and index) from the input file.
// The "read" symbol is placed in the pointer to sym (pass by reference).
// The "read" index is placed in the pointer to index (pass by reference).
// In reality, a block of pairs is read into a buffer.
// An index keeps track of the current bit in the buffer.
// Once all bits have been processed, another block is read.
// The first 8 bits of the pair constitute the symbol, starting from the LSB.
// The next bit_len bits constitutes the index, starting from the the LSB.
// Returns true if there are pairs left to read in the buffer, else false.
// There are pairs left to read if the read index is not STOP_INDEX.
//
// infile:  File descriptor of the input file to read from.
// sym:     Pointer to memory which stores the read symbol.
// index:   Pointer to memory which stores the read index.
// bit_len: Length in bits of the index to read.
// returns: True if there are pairs left to read, false otherwise.
bool read_pair(int infile, uint16_t *code, uint8_t *sym, uint8_t bit_len) {
  //printf("read pair begin\n");
  total_bits += 8 + bit_len;
  *code = 0;
  *sym = 0;

  for (uint8_t i = 0; i < bit_len; i++) {
    //printf("first %d\n", i);
    if (!bit_cnt) {
      read_bytes(infile, bit_buff, 4096);
    }

    if (bit_buff[bit_cnt / 8] & (1 << (bit_cnt % 8))) {
      *code |= (1 << (i % 16));
      //printf("second code %d\n", code);
    } else {
      *code &= ~(1 << (i % 16));
      //printf("third code %d\n", code);
    }
    bit_cnt = (bit_cnt + 1) % (4096 * 8);
  }

  for (uint8_t i = 0; i < 8; i++) {
    //printf("second %d\n", i);
    if (!bit_cnt) {
      read_bytes(infile, bit_buff, 4096);
    }

    if (bit_buff[bit_cnt / 8] & (1 << (bit_cnt % 8))) {
      *sym |= (1 << (i % 8));
    } else {
      *sym &= ~(1 << (i % 8));
    }
    bit_cnt = (bit_cnt + 1) % (4096 * 8);
  }

  return *code != STOP_CODE;
}
//
// Buffers a Word, or more specifically, the symbols of a Word.
// Each symbol of the Word is placed into a buffer.
// The buffer is written out when it is filled.
//
// outfile: File descriptor of the output file to write to.
// w:       Word to buffer.
// returns: Void.
void buffer_word(int outfile, Word *w) {
  if (!w) {
    //printf("Inputting null word\n");
    return;
  }

  total_syms += w->len;

  for (uint32_t i = 0; i < w->len; i++) {
    sym_buff[sym_cnt] = w->syms[i];
    sym_cnt++;
    if (sym_cnt == 4096) {
      write_bytes(outfile, sym_buff, 4096);
      sym_cnt = 0;
    }
  }
}

//
// Writes out any remaining symbols in the buffer.
//
// outfile: File descriptor of the output file to write to.
// returns: Void.
void flush_words(int outfile) {
  if (sym_cnt) {
    write_bytes(outfile, sym_buff, sym_cnt);
  }
}
