CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c23
DEBUG_FLAGS=-ggdb -fsanitize=address
RELEASE_FLAGS=-O3
INCLUDE=-Isrc/include -Isrc/include/raylib
LIBRARY=-Lsrc/lib
LIB=-lcrypto -lssl -lm -ldl -lpthread -lGL -lrt -lX11
SRC=src/tetris.c src/tetris_utils.c src/tetris_debug.c src/tetris_hashmap.c src/tetris_ai.c src/tetris_ui.c
MAIN=src/main.c
OUT=-o out/tetris
TEST_OUT=-o out/tetris-test
UNAME_S=$(shell uname)

ifeq ($(UNAME_S),Linux)
	OS_LIB=-lraylib-linux
else ifeq ($(UNAME_S),Darwin)
	OS_LIB=-lraylib-mac
endif

pre:
	mkdir -p out
	cd out && ln -sf ../src/assets .

debug: pre
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(INCLUDE) $(LIBRARY) $(OUT) $(SRC) $(MAIN) $(LIB) $(OS_LIB)

release: pre
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) $(INCLUDE) $(LIBRARY) $(OUT) $(SRC) $(MAIN) $(LIB) $(OS_LIB)

test: pre
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(INCLUDE) $(LIBRARY) $(TEST_OUT) $(SRC) tests/test_$(NAME).c $(LIB) $(OS_LIB)

runDebug: debug
	cd out && ./tetris

runTest: test
	cd out && ./tetris-test
