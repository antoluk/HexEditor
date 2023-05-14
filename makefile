CC = gcc
CFLAGS = -W -Wall -Wextra -std=c11 -pedantic -D_DEFAULT_SOURCE -D_BSD_SOURCE
path=./src/
.PHONY: clean
all: hexed
hexed: src/main.c makefile
	$(CC) $(CFLAGS) $(path)main.c $(path)png_analyse.c $(path)signatures.c $(path)event_handler.c $(path)menu.c $(path)stack.c $(path)file_work.c $(path)elf_analyse.c $(path)analyse.c -o hexed -lncurses
clean:
	rm hexed