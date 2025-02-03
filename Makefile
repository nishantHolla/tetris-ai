CC=gcc
CFLAGS=-Wall -Wextra -pedantic
DEBUG_FLAGS=-ggdb -fsanitize=address
RELEASE_FLAGS=-O3
INCLUDE=-Isrc/include -Isrc/include/raylib
LIB=-lcrypto -lssl -lm -ldl -lpthread -lGL -lrt -lX11 -lraylib
SRC=src/tetris.c src/tetris_utils.c src/tetris_debug.c src/tetris_hashmap.c src/tetris_ai.c src/tetris_ui.c
MAIN=src/main.c
OUT=-o out/tetris
TEST_OUT=-o out/tetris
UNAME_S=$(shell uname)

pre:
	mkdir -p out
	cd out && cp -r ../src/assets .

debug: pre
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(INCLUDE) $(OUT) $(SRC) $(MAIN) $(LIB)

release: pre
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) $(INCLUDE) $(OUT) $(SRC) $(MAIN) $(LIB) -lm

test: pre
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(INCLUDE) $(TEST_OUT) $(SRC) tests/test_$(NAME).c $(LIB)

runDebug: debug
	cd out && ./tetris

runTest: test
	cd out && ./tetris-test
