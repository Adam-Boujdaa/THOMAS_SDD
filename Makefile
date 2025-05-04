CC = gcc
CFLAGS = -Wall -g -ggdb -std=c99
LIBFLAGS = 
INCLUDE = include/
SRC = $(wildcard src/*.c)
MOVE = mv
EXC = ./bin/main
LIB = lib/
BIN = bin/*

all:
	$(MAKE) compile -s
	$(MAKE) run -s

compile:
	@$(CC) $(CFLAGS) -I $(INCLUDE) -c $(SRC) $(LIBFLAGS)
	@$(MOVE) *.o $(LIB)
	@$(CC) $(CFLAGS) -o $(EXC) $(LIB)*.o $(LIBFLAGS)

run:
	@$(EXC)

clean:
	rm $(BIN) $(LIB)*
