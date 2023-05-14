//
// Created by antoluk on 24.4.23.
//

#ifndef HEXEDITOR_MENU_H
#define HEXEDITOR_MENU_H

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "event_handler.h"

extern WINDOW *main_win;
extern char *filename;
extern char *filepath;
extern int file_size;
extern int letter;
extern struct node *stack;

void render_menu();

void go_to();

void exit_win();

#endif //HEXEDITOR_MENU_H
