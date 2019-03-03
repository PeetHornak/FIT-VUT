#Subor: Makefile
#Projekt: 2. uloha IOS
#Datum: 1.5.2018
#Autor: Peter Hornak - xhorna14
#Poznamky: make - Vytvori spustitelny program
#Poznamky: clean - Vymaze z priecinka objektove subory, spustitelnu binarku a output file
#Poznamky: zip - Vytvori spustitelny program a zabali subory na odovzdanie projektu
#Poznamky: run - Vytvori spustitelny program, spusti ho,
#		   skontroluje syntax ak je skript v priecinku a vypise output file

CC = gcc
CFLAGS = -std=gnu99 -Wall -Wextra -Werror -pedantic

all: proj2

proj2: proj2.o
	$(CC) $(CFLAGS) proj2.o -o proj2 -lpthread -lrt

proj2.o: proj2.c proj2.h
	$(CC) $(CFLAGS) -c proj2.c -o proj2.o

clean:
	rm -f *.o proj2 proj2.out

zip: proj2
	zip proj2.zip proj2.c proj2.h Makefile

run: proj2
	./proj2 5 2 2 10 && ./check-syntax.sh proj2.out && cat proj2.out
