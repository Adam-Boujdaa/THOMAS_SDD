CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -I/usr/include/cjson
LDFLAGS = -lssl -lcrypto -lcjson

SRC = src/main.c src/trajet.c src/voyageur.c src/place.c src/auth.c src/json_export.c src/menu.c
OBJ = $(SRC:.c=.o)

BIN = bin/programme

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(BIN)

.PHONY: all clean
