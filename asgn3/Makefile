FLAGS=-Wall -Wextra -Werror -pedantic
CC=clang $(CFLAGS)

all	:	tower
tower	:	tower.o stack.o
	$(CC) -o tower stack.o tower.o
stack.o	:	stack.c	stack.h
	$(CC) -c stack.c
tower.o	:	tower.c
	$(CC) -c tower.c
clean	:
	rm -f tower tower.o stack.o
infer	:
	make clean; infer-capture -- make; infer-capture -- make; infer-analyze -- make
