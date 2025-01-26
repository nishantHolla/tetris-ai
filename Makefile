CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c23
DEBUG_FLAGS=-ggdb -fsanitize=address
RELEASE_FLAGS=-O3
INCLUDE=-Isrc/include
LIBRARY=
LIB=-lcrypto -lssl
SRC=src/tetris.c src/tetris_utils.c src/tetris_debug.c src/tetris_hashmap.c
MAIN=src/main.c
OUT=-o out/tetris
TEST_OUT=-o out/tetris-test

pre:
	mkdir -p out

debug: pre
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(INCLUDE) $(LIBRARY) $(OUT) $(SRC) $(MAIN) $(LIB)

release: pre
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) $(INCLUDE) $(LIBRARY) $(OUT) $(SRC) $(MAIN) $(LIB)

test: pre
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(INCLUDE) $(LIBRARY) $(TEST_OUT) $(SRC) tests/test_$(NAME).c $(LIB)

runDebug: debug
	cd out && ./tetris

runTest: test
	cd out && ./tetris-test
