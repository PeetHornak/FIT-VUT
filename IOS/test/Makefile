CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic
LDFLAGS = -lpthread

all: test

test: test.o
	$(CC) $(CFLAGS) test.o -o test -lpthread

test.o: test.c
	$(CC) $(CFLAGS) -c test.c -o test.o
