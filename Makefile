CC = gcc
CFLAGS = -Wall

all: search

search: search.c
	$(CC) $(CFLAGS) -o search search.c

clean:
	rm -f search

.PHONY: all cleans