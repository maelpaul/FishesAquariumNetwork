CFLAGS = -Wall -Wextra -std=c99

SRC_DIR = src
TEST_DIR = test

INC_DIR = -I$(SRC_DIR) -I$(TEST_DIR)

all : build test

build : prompt

test : test_fish test_view

$(SRC_DIR)/parser.o : $(SRC_DIR)/parser.c
	gcc ${CFLAGS} ${INC_DIR} $< -c -o $@

$(SRC_DIR)/fish.o : $(SRC_DIR)/fish.c
	gcc ${CFLAGS} ${INC_DIR} $< -c -o $@

$(SRC_DIR)/view.o : $(SRC_DIR)/view.c
	gcc ${CFLAGS} ${INC_DIR} $< -c -o $@

$(SRC_DIR)/aquarium.o : $(SRC_DIR)/aquarium.c
	gcc ${CFLAGS} ${INC_DIR} $< -c -o $@

prompt: $(SRC_DIR)/prompt.c $(SRC_DIR)/parser.o
	gcc $(CFLAGS) ${INC_DIR} $^ -o $@

server: $(SRC_DIR)/server.c
	gcc ${CFLAGS} ${INC_DIR} $^ -o $@

test_fish : $(SRC_DIR)/fish.o
	gcc ${CFLAGS} ${INC_DIR} $(TEST_DIR)/test_fish.c $^ -o $@

test_view : $(SRC_DIR)/view.o
	gcc ${CFLAGS} ${INC_DIR} $(TEST_DIR)/test_view.c $^ -o $@

clean :
	rm -f $(TST_DIR)/*.o
	rm -f $(SRC_DIR)/*.o
	rm -f main server test_parser test_view prompt test_fish
