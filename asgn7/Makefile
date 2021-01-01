FLAGS=-Wall -Wextra -Werror -Wpedantic -g 
CC=clang $(CFLAGS)

all	:	encode decode
encode :	encode.o trie.o word.o io.o
	$(CC) -o encode encode.o trie.o word.o io.o -lm
encode.o	:	encode.c
	$(CC) -c encode.c
decode	:	decode.o trie.o word.o io.o
	$(CC) -o decode decode.o trie.o word.o io.o -lm
decode.o	:	decode.c
	$(CC) -c decode.c
trie.o	:	trie.c trie.h
	$(CC) -c trie.c
word.o	:	word.c word.h
	$(CC) -c word.c
io.o	:	io.c io.h
	$(CC) -c io.c
clean	:
	rm -rf encode encode.o decode decode.o trie.o word.o io.o infer-out 
infer	:
	make clean; infer-capture -- make; infer-analyze -- make
