CC = gcc
CFLAGS = -Wall -g -ggdb -std=c99 -Iinclude/ -Ilib/
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
	@$(CC) $(CFLAGS) -I$(INCLUDE) -c $(SRC) $(LIBFLAGS)
	@$(MOVE) *.o $(LIB)
	@$(CC) $(CFLAGS) -o $(EXC) $(LIB)*.o $(LIBFLAGS)

run:
	@$(EXC)

clean:
	rm $(BIN) $(LIB)*

test: compile
	@$(CC) $(CFLAGS) -I$(INCLUDE) -o ./bin/test_export src/export_voyageurs.c $(LIB)*.o $(LIBFLAGS)
	@./bin/test_export
