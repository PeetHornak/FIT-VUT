#Subor: Makefile
#Projekt: IJC DU2
#Datum: 24.4.2018
#Autor: Peter Hornak - xhorna14
#Poznamky: make - Vytvori vsekty spustitelne subory, objektove subory, staticku a zdielanu kniznicu
#Poznamky: clean - Vymaze vsetky vygenerovane subory
#Poznamky: zip - Zabali subory na odovzdanie projektu

CC=gcc
CFLAGS= -pedantic -O2 -Wall -g -std=c99
CXX=g++
CXXFLAGS= -pedantic -Wall -O2 -g -std=c++11

CTARGET:=$(shell ls htab_*.c)

OTARGET:=$(CTARGET:.c=.o)

#DYNAMIC:=$(patsubst %.o, %-dynamic.o, $(OTARGET))

.PHONY: all

all: tail tail2 wordcount wordcount-dynamic

htab_%.o: htab_%.c
	$(CC) $(CFLAGS) -fPIC -c $^ -o $@

io.o: io.c io.h

wordcount.o: wordcount.c

libhtab.a: $(OTARGET)
	ar csr $@ $(OTARGET)
	ranlib $@

libhtab.so: $(OTARGET)
	$(CC) $(CFLAGS) -shared -fPIC  $(OTARGET) -o $@

tail: tail.c

tail2: tail2.cc

wordcount: wordcount.o io.o libhtab.a

wordcount-dynamic: wordcount.o io.o libhtab.so
	$(CC) $(CFLAGS) -L. $^ -o $@ -lhtab

clean:
	rm -f *.o tail tail2 libhtab.a libhtab.so wordcount wordcount-dynamic

zip:
	zip xhorna14.zip *.c *.cc *.h Makefile
