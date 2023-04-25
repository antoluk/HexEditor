//
// Created by antoluk on 24.4.23.
//

#include "menu.h"
#include "event_handler.h"

#define STYLE_CURSOR 14


void render_menu() {
    int x = 0, y = 0;
    getmaxyx(stdscr, y, x);
    wclear(stdscr);
    wattron(stdscr, COLOR_PAIR(STYLE_CURSOR));
    wmove(stdscr, y - 1, 0);
    for (int i = 0; i < x; i++)wprintw(stdscr, " ");
    wmove(stdscr, y - 1, 0);
    wprintw(stdscr, "[F1]exit [F2]goto [F3]change ASCII [F4]change HEX [F5]back");
    if (change_mod == HEX)wprintw(stdscr, "     HEX change mod");
    else wprintw(stdscr, "     ASCII change mod");
    wattroff(stdscr, COLOR_PAIR(STYLE_CURSOR));
}

