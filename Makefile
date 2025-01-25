CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c23
INCLUDE=-Isrc/include
LIBRARY=
LIB=
SRC=src/tetris.c src/tetris_utils.c src/tetris_debug.c
MAIN=src/main.c
OUT=-o out/tetris
TEST_OUT=-o out/tetris-test

pre:
	mkdir -p out

debug: pre
	$(CC) $(CFLAGS) -ggdb $(INCLUDE) $(LIBRARY) $(OUT) $(SRC) $(MAIN) $(LIB)

release: pre
	$(CC) $(CFLAGS) -O3 $(INCLUDE) $(LIBRARY) $(OUT) $(SRC) $(MAIN) $(LIB)

test: pre
	$(CC) $(CFLAGS) -ggdb $(INCLUDE) $(LIBRARY) $(TEST_OUT) $(SRC) tests/test_$(NAME).c $(LIB)

runDebug: debug
	cd out && ./tetris

runTest: test
	cd out && ./tetris-test
