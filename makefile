CC = gcc
CFLAGS = -W -Wall -Wextra -std=c11 -pedantic -D_DEFAULT_SOURCE -D_BSD_SOURCE
.PHONY: clean
all: cringe
cringe: main.c makefile
	$(CC) $(CFLAGS) main.c analyse.c png_analyse.c signatures.c graphic.c -o cringe -lncurses
clean:
	rm cringe