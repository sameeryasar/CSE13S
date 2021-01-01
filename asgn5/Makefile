CFLAGS==Wall -Wextra -Werror -Wpedantic -std=c99
CC=clang $(CLFAGS)

sorting		:	sorting.o bubble.o binary.o quick.o shell.o
		$(CC) -o sorting sorting.o bubble.o binary.o quick.o shell.o
bubble.o	:	bubble.c bubble.h
		$(CC) -c bubble.c
binary.o	:	binary.c binary.h
		$(CC) -c binary.c
quick.o		:	quick.c quick.h
		$(CC) -c quick.c
shell.o		:	shell.c shell.h
		$(CC) -c shell.c
sorting.o	:	sorting.c
		$(CC) -c sorting.c
clean		:
		rm -f sorting bubble.o binary.o quick.o shell.o sorting.o
infer		:
		make clean; infer-capture -- make; infer-analyze -- make
valgrind	:
		valgrind ./sorting -A
