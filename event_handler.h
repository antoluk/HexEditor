//
// Created by antoluk on 5.4.23.
//

#ifndef HEXEDITORSECONDTRY_EVENT_HANDLER_H
#define HEXEDITORSECONDTRY_EVENT_HANDLER_H

#include <unistd.h>
#include <ncurses.h>

typedef struct {
    int x;
    int y;
} Point;

extern Point cur, ccur,inFile;

int event();
int kbhit();
#endif //HEXEDITORSECONDTRY_EVENT_HANDLER_H
