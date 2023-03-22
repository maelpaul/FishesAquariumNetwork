CFLAGS = -Wall -Wextra -std=c99

all : parser

parser : parser.c
	gcc ${CFLAGS} $< -o $@

clean :
	rm *.o parser