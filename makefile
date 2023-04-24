CC = gcc
CFLAGS = -W -Wall -Wextra -std=c11 -pedantic -D_DEFAULT_SOURCE -D_BSD_SOURCE
.PHONY: clean
all: cringe
cringe: main.c makefile
	$(CC) $(CFLAGS) main.c png_analyse.c signatures.c event_handler.c menu.c -o cringe -lncurses
clean:
	rm cringe