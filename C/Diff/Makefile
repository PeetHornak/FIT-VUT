CC=gcc
CDFLAGS= -std=c99 -pedantic -Werror -Wall
LDFLAGS= -lm
PROJ_NAME= diff

.PHONY = all

all: $(PROJ_NAME)

$(PROJ_NAME): $(PROJ_NAME).o

$(PROJ_NAME).o: $(PROJ_NAME).c

clean:
	-rm $(PROJ_NAME) $(PROJ_NAME).o
