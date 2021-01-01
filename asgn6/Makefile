FLAGS=-Wall -Wextra -Werror -pedantic -std=c99 -g
CC=clang $(CFLAGS)

hatterspeak	:	hatterspeak.o utility.o ll.o bf.o bv.o speck.o hash.o parser.o
	$(CC) -o hatterspeak hatterspeak.o utility.o ll.o bf.o bv.o speck.o hash.o parser.o
utility.o	:	utility.c utility.h
	$(CC) -c utility.c
ll.o	:	ll.c ll.h
	$(CC) -c ll.c
bf.o	:	bf.c bf.h
	$(CC) -c bf.c
bv.o	:	bv.c bv.h
	$(CC) -c bv.c
speck.o	:	speck.c speck.h
	$(CC) -c speck.c
hash.o	:	hash.c hash.h
	$(CC) -c hash.c
parser.o	:	parser.c parser.c
	$(CC) -c parser.c
hatterspeak.o	:	hatterspeak.c
	$(CC) -c hatterspeak.c
clean	:
	rm -rf hatterspeak hatterspeak.o utility.o ll.o hash.o speck.o bf.o bv.o infer-out
infer	:
	make clean; infer-capture -- make; infer-analyze -- make	 
