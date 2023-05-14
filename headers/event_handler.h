//
// Created by antoluk on 5.4.23.
//

#ifndef HEXEDITOR_EVENT_HANDLER_H
#define HEXEDITOR_EVENT_HANDLER_H

#include <unistd.h>
#include <ncurses.h>
#include <stdlib.h>
#include "menu.h"

#define STYLE_CURSOR 14

typedef struct {
    int x;
    int y;
} Point;
typedef enum {
    HEX,
    ASCII
} change;

extern change change_mod;
extern Point cur, ccur, inFile;
extern int dump_offset;

int event();

int kbhit();

void move_left();

void move_right();

void move_up();

void move_down();


#endif //HEXEDITOR_EVENT_HANDLER_H
