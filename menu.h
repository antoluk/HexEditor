//
// Created by antoluk on 24.4.23.
//

#ifndef HEXEDITORSECONDTRY_MENU_H
#define HEXEDITORSECONDTRY_MENU_H

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "event_handler.h"

extern WINDOW *main_win;
extern char *filename;
extern int file_size;
extern int letter;
extern struct node *stack;

void render_menu();

void go_to();

void exit_win();

#endif //HEXEDITORSECONDTRY_MENU_H
