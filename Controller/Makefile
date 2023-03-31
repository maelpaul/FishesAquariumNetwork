CFLAGS = -Wall -Wextra -std=c99

all : build

build : parser.o main test_parser prompt test_fish

parser.o : parser.c
	gcc ${CFLAGS} $< -c

main : main.c parser.o
	gcc ${CFLAGS} $^ -o $@

test_parser : test_parser.c parser.o
	gcc ${CFLAGS} $^ -o $@

prompt: prompt.c parser.o
	gcc $(CFLAGS) $^ -o $@

server: server.c
	gcc ${CFLAGS} $^ -o $@

fish.o : fish.c
	gcc ${CFLAGS} $< -c

test_fish : test_fish.c fish.o
	gcc ${CFLAGS} $^ -o $@

test_view : test_view.c view.o
	gcc ${CFLAGS} $^ -o $@

view.o : view.c
	gcc ${CFLAGS} $< -c

aquarium.o : aquarium.c
	gcc ${CFLAGS} $< -c

clean :
	rm *.o
	rm -f main server test_parser test_view prompt test_fish
