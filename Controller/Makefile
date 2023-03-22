CFLAGS = -Wall -Wextra -std=c99

all : build

build : parser.o main

parser.o : parser.c
	gcc ${CFLAGS} $< -c

main : parser.o
	gcc ${CFLAGS} $@.c $< -o $@

clean :
	rm *.o