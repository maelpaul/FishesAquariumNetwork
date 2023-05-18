CFLAGS = -Wall -Wextra -std=c99

SRC_DIR = src
TEST_DIR = test

INC_DIR = -I$(SRC_DIR) -I$(TEST_DIR)

all : build test

build : server client

test : test_fish test_view test_aquarium

$(SRC_DIR)/parser.o : $(SRC_DIR)/parser.c
	gcc ${CFLAGS} ${INC_DIR} $< -c -o $@

$(SRC_DIR)/thread.o : $(SRC_DIR)/thread.c
	gcc ${CFLAGS} ${INC_DIR} $< -c -o $@

$(SRC_DIR)/command_fish.o : $(SRC_DIR)/command_fish.c
	gcc ${CFLAGS} ${INC_DIR} $< -c -o $@

$(SRC_DIR)/fish.o : $(SRC_DIR)/fish.c
	gcc ${CFLAGS} ${INC_DIR} $< -c -o $@

$(SRC_DIR)/prompt.o : $(SRC_DIR)/prompt.c $(SRC_DIR)/prompt_commands.o
	gcc ${CFLAGS} ${INC_DIR} $< -c -o $@

$(SRC_DIR)/view.o : $(SRC_DIR)/view.c
	gcc ${CFLAGS} ${INC_DIR} $< -c -o $@

$(SRC_DIR)/aquarium.o : $(SRC_DIR)/aquarium.c
	gcc ${CFLAGS} ${INC_DIR} $< -c -o $@

$(SRC_DIR)/server_utils.o : $(SRC_DIR)/server_utils.c
	gcc ${CFLAGS} ${INC_DIR} $< -c -o $@

$(SRC_DIR)/command_getfish.o : $(SRC_DIR)/command_getfish.c
	gcc ${CFLAGS} ${INC_DIR} $< -c -o $@

$(SRC_DIR)/init_client.o : $(SRC_DIR)/init_client.c
	gcc ${CFLAGS} ${INC_DIR} $< -c -o $@

$(SRC_DIR)/prompt_commands.o : $(SRC_DIR)/prompt_commands.c
	gcc ${CFLAGS} ${INC_DIR} $< -c -o $@

server: $(SRC_DIR)/server.c $(SRC_DIR)/server_utils.o $(SRC_DIR)/aquarium.o $(SRC_DIR)/view.o $(SRC_DIR)/fish.o $(SRC_DIR)/thread.o $(SRC_DIR)/command_getfish.o $(SRC_DIR)/prompt_commands.o $(SRC_DIR)/command_fish.o $(SRC_DIR)/init_client.o $(SRC_DIR)/parser.o $(SRC_DIR)/prompt_commands.o $(SRC_DIR)/prompt.o
	gcc ${CFLAGS} ${INC_DIR} $^ -o $@ -lm -lpthread

client: $(SRC_DIR)/client.c
	gcc ${CFLAGS} ${INC_DIR} $^ -o $@ -lpthread

test_fish : $(SRC_DIR)/fish.o
	gcc ${CFLAGS} ${INC_DIR} $(TEST_DIR)/test_fish.c $^ -o $@

test_view : $(SRC_DIR)/view.o
	gcc ${CFLAGS} ${INC_DIR} $(TEST_DIR)/test_view.c $^ -o $@

test_aquarium: $(SRC_DIR)/fish.o $(SRC_DIR)/view.o $(SRC_DIR)/aquarium.o
	gcc ${CFLAGS} ${INC_DIR} $(TEST_DIR)/test_aquarium.c $^ -o $@

clean :
	rm -f $(TST_DIR)/*.o
	rm -f $(SRC_DIR)/*.o
	rm -f server test_view prompt test_fish test_aquarium client
