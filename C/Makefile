CC=gcc
CFLAGS= -std=c99 -pedantic -Werror -Wall -g
LDFLAGS= -lm
PROJ_NAME= proj

.PHONY = all

all: $(PROJ_NAME)

$(PROJ_NAME): $(PROJ_NAME).o

$(PROJ_NAME).o: $(PROJ_NAME).c

clean:
	-rm $(PROJ_NAME) $(PROJ_NAME).o
