CFLAGS = -Wall -Wextra -std=c99

all : build

build : parser.o main test_parser server

parser.o : parser.c
	gcc ${CFLAGS} $< -c

main : main.c parser.o
	gcc ${CFLAGS} $^ -o $@

test_parser : test_parser.c parser.o
	gcc ${CFLAGS} $^ -o $@

server: server.c
	gcc ${CFLAGS} $^ -o $@

clean :
	rm *.o
	rm main
	rm server
	rm test_parser