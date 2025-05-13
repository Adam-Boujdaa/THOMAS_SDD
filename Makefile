CC = gcc
CFLAGS = -Wall -g -ggdb -std=c99 -Iinclude/ -Ilib/cJSON/
LIBFLAGS = -lm -lssl -lcrypto
INCLUDE = include/
SRC = $(filter-out src/voyageur.c, $(wildcard src/*.c)) # Exclude voyageur.c from redundant compilation
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
	@$(CC) $(CFLAGS) -I$(INCLUDE) -Ilib/cJSON -o ./bin/test_export_db test/test_export_db.c $(filter-out $(LIB)main.o, $(wildcard $(LIB)*.o)) $(LIBFLAGS)
	@./bin/test_export_db
