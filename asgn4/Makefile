FLAGS=-Wall -Wextra -Werror -pedantic
CC=clang $(CFLAGS)

all	:	sequence
sequence	:	sequence.o bv.o sieve.o
	$(CC) -o sequence sequence.o bv.o sieve.o -lm
bv.o	:	bv.c bv.h
	$(CC) -c bv.c
sieve.o	:	sieve.c sieve.h
	$(CC) -c sieve.c
sequence.o	:	sequence.c
	$(CC) -c sequence.c
clean	:
	rm -f sequence sequence.o bv.o sieve.o
infer	:
	make clean; infer-capture -- make; infer-analyze -- make
