//
// Created by antoluk on 24.4.23.
//

#include "menu.h"
#include "stack.h"
#define STYLE_CURSOR 14


void render_menu() {
    int x , y , x_m ;
    int offset = strlen(filename) - 50;
    getmaxyx(stdscr, y, x);
    x_m = getmaxx(main_win);
    wclear(stdscr);
    char *filename_str = malloc(strlen(filename));
    strcpy(filename_str, filename);
    if (strlen(filename) > 50) {
        filename_str += offset;
        filename_str[0] = '.';
        filename_str[1] = '.';
        filename_str[2] = '.';
    }
    mvwprintw(main_win, 0, x_m / 2 - strlen(filename_str) / 2, "%s", filename_str);
    wattron(stdscr, COLOR_PAIR(STYLE_CURSOR));
    wmove(stdscr, y - 1, 0);
    for (int i = 0; i < x; i++)wprintw(stdscr, " ");
    wmove(stdscr, y - 1, 0);
    wprintw(stdscr, "[F1]exit [F2]goto [F3]change ASCII [F4]change HEX [F5]back");
    if (change_mod == HEX)wprintw(stdscr, "     HEX change mod");
    else wprintw(stdscr, "     ASCII change mod");
    wattroff(stdscr, COLOR_PAIR(STYLE_CURSOR));
    if (strlen(filename) > 50) {
        filename_str -= offset;
    }
    free(filename_str);
}

void go_to() {
    WINDOW *go_to = newwin(6, 25, 9, 27);
    cbreak();
    nodelay(go_to, true);
    char address[8] = "        ";
    int j = 0;
    int inp = -1;
    curs_set(0);
    do {
        if (((inp >= '0' && inp <= '9') || (inp >= 'a' && inp <= 'f') || (inp >= 'A' && inp <= 'F')) && j <= 7) {
            if (inp >= 'a' && inp <= 'f')inp -= 32;
            address[j++] = (char) inp;
        } else if (inp == KEY_BACKSPACE && j > 0) {
            address[--j] = ' ';
        }
        box(go_to, 0, 0);
        mvwprintw(go_to, 2, 6, "Enter address");
        wattron(go_to, COLOR_PAIR(STYLE_CURSOR));
        for (int i = 0; i < 8; i++)mvwprintw(go_to, 4, 8 + i, "%c", address[i]);
        wattroff(go_to, COLOR_PAIR(STYLE_CURSOR));
        refresh();
        wrefresh(go_to);
        fflush(stdin);
    } while ((inp = getch()) != '\n');
    long adr = strtol(address, NULL, 16);
    if (adr < file_size) {
        while (inFile.y < adr) {
            move_down();
        }
        while (inFile.y > adr) {
            move_up();
        }
        while (inFile.x > adr % 16) {
            move_left();
        }
        while (inFile.x < adr % 16) {
            move_right();
        }
        if (letter == 2)move_left();
    }

    curs_set(1);
    delwin(go_to);
}

void exit_win() {
    WINDOW *exit_win = newwin(6, 25, 9, 27);
    cbreak();
    nodelay(exit_win, true);
    char choice[8] = "yes   no";
    int save = 1;
    int j = 0;
    int inp = -1;
    curs_set(0);
    do {
        if (inp == KEY_RIGHT)save = 0;
        if (inp == KEY_LEFT)save = 1;
        box(exit_win, 0, 0);
        mvwprintw(exit_win, 2, 6, "Save changes?");
        if (save) {
            int i = 0;
            wattron(exit_win, COLOR_PAIR(STYLE_CURSOR));
            for (; i < 3; i++)mvwprintw(exit_win, 4, 8 + i, "%c", choice[i]);
            wattroff(exit_win, COLOR_PAIR(STYLE_CURSOR));
            for (; i < 8; i++)mvwprintw(exit_win, 4, 8 + i, "%c", choice[i]);
        } else {
            int i = 0;
            for (; i < 6; i++)mvwprintw(exit_win, 4, 8 + i, "%c", choice[i]);
            wattron(exit_win, COLOR_PAIR(STYLE_CURSOR));
            for (; i < 8; i++)mvwprintw(exit_win, 4, 8 + i, "%c", choice[i]);
            wattroff(exit_win, COLOR_PAIR(STYLE_CURSOR));
        }
        refresh();
        wrefresh(exit_win);
        fflush(stdin);
    } while ((inp = getch()) != '\n');

if(!save) {
    while (stack)
        pop(&stack);
}
    curs_set(1);
    delwin(exit_win);
}