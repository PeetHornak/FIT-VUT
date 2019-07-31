# Makefile for compiler project.
# Written by:
# 	xpatri00 Nikolas Patrik
# 	xjance00 Matej Jancek
# 	xhubin03 Robert Hubinak
# 	xhorna14 Peter Hornak
# Makefile version: 4.2.1
	
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -g
SHELL = /bin/bash

vpath %.c src:tests
vpath %.h src/header
vpath %.o build

b_tests = test_gcollector test_parseexp test_scanner test_symtable

.PHONY: all clean commit tests zip

object_file = main.o garbagecollector.o generate.o instrlist.o parseexp.o parser.o queue.o scanner.o stack.o symtable.o

ifj18: $(object_file)
	$(CC) $^ -o $@

tests: $(b_tests)
	run-parts tests/

test_gcollector: test_gcollector.o garbagecollector.o
	$(CC) $^ -o tests/$@

test_parseexp: test_parseexp.o scanner.o parseexp.o garbagecollector.o queue.o generate.o instrlist.o symtable.o
	$(CC) $^ -o tests/$@

test_scanner: test_scanner.o scanner.o queue.o garbagecollector.o
	$(CC) $^ -o tests/$@

test_symtable: test_symtable.o symtable.o garbagecollector.o
	$(CC) $^ -o tests/$@

test_parser: test_parser.o parser.o garbagecollector.o
	$(CC) $^ -o tests/$@

build/test_gcollector.o: test_gcollector.c garbagecollector.h
	$(CC) $(CFLAGS) -c $< -o $@

build/test_parseexp.o: test_parseexp.c scanner.h parseexp.h garbagecollector.h
	$(CC) $(CFLAGS) -c $< -o $@

build/test_scanner.o: test_scanner.c scanner.h 
	$(CC) $(CFLAGS) -c $< -o $@

build/test_symtable.o: test_symtable.c symtable.h
	$(CC) $(CFLAGS) -c $< -o $@

build/test_parser.o: test_parser.c parser.h
	$(CC) $(CFLAGS) -c $< -o $@

build/main.o: main.c parser.h symtable.h scanner.h generate.h
	$(CC) $(CFLAGS) -c $< -o $@

build/garbagecollector.o: garbagecollector.c garbagecollector.h
	$(CC) $(CFLAGS) -c $< -o $@

build/scanner.o: scanner.c scanner.h queue.h garbagecollector.h
	$(CC) $(CFLAGS) -c $< -o $@

build/parseexp.o: parseexp.c parseexp.h scanner.h garbagecollector.h
	$(CC) $(CFLAGS) -c $< -o $@

build/symtable.o: symtable.c symtable.h
	$(CC) $(CFLAGS) -c $< -o $@

build/queue.o: queue.c queue.h scanner.h
	$(CC) $(CFLAGS) -c $< -o $@

build/stack.o: stack.c stack.h
	$(CC) $(CFLAGS) -c $< -o $@

build/parser.o: parser.c parser.h scanner.h symtable.h parseexp.h garbagecollector.h instrlist.h generate.h
	$(CC) $(CFLAGS) -c $< -o $@

build/generate.o: generate.c generate.h instrlist.h symtable.h garbagecollector.h
	$(CC) $(CFLAGS) -c $< -o $@

build/instrlist.o: instrlist.c instrlist.h 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(addprefix tests/,$(b_tests)) build/*.o main

	
