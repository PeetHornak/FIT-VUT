# Subor: Makefile
# Projekt: IJC DU1
# Datum: 20.3.2018
# Autor: Peter Hornak - xhorna14
# Poznamky: Run - spusti primes a primes-i
 
SHELL = /bin/bash

CC=gcc
CFLAGS= -m32 -pedantic -O2 -Wall -std=c99
LDFLAGS= -m32 -lm

.PHONY: all
all: primes primes-i steg-decode

primes: primes.o eratosthenes.o error.o

primes.o: primes.c bit_array.h eratosthenes.h error.h 

eratosthenes.o: eratosthenes.c bit_array.h error.h eratosthenes.h

primes-i: primes-i.o eratosthenes-i.o error.o

primes-i.o: primes.c bit_array.h error.h eratosthenes.h
	$(CC) $(CFLAGS) -c -DUSE_INLINE $< -o $@

eratosthenes-i.o: eratosthenes.c bit_array.h error.h eratosthenes.h
	$(CC) $(CFLAGS) -c -DUSE_INLINE $< -o $@

error.o: error.c error.h

steg-decode: ppm.o steg-decode.o error.o eratosthenes.o

steg-decode.o: steg-decode.c ppm.h error.h bit_array.h eratosthenes.h

ppm.o: ppm.c ppm.h error.h

run: primes primes-i
	time ./primes
	time ./primes-i

clean:
	-rm -f *.o primes primes-i steg-decode

zip:
	zip xhorna14.zip *.h *.c Makefile
