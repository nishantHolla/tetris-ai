CC=gcc
CFLAGS=-Wall -Wextra -pedantic
INCLUDE=-Isrc/include
LIBRARY=
LIB=
SRC=
MAIN=src/main.c
OUT=-o out/tetris

pre:
	mkdir -p out

debug: pre
	$(CC) $(CFLAGS) -ggdb $(INCLUDE) $(LIBRARY) $(OUT) $(SRC) $(MAIN) $(LIB)

release: pre
	$(CC) $(CFLAGS) -O3 $(INCLUDE) $(LIBRARY) $(OUT) $(SRC) $(MAIN) $(LIB)

runDebug: debug
	cd out && ./tetris
