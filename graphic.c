//
// Created by antoluk on 24.3.23.
//

#include "graphic.h"

WINDOW *main_win = NULL, *analyse_win = NULL;

void win_init() {
    initscr();
    cbreak();
    noecho();
    int x, y;
    getmaxyx(stdscr, y, x);
    main_win = newwin(y - 5, x - 1, 0, 1);
    analyse_win = newwin(5, x - 1, y - 5, 1);
    touchwin(main_win);
    box(main_win, 0, 0);
    box(analyse_win, 0, 0);
    print_dump();
    while (true) {
        refresh();
        wrefresh(main_win);
        wrefresh(analyse_win);
        flushinp();
        fflush(stdin);
        if (getch() == 'q') break;
    }
    endwin();
}

void print_dump() {
    FILE *fp;
    unsigned long size;
    int fx = 0,x=0,cur_str=1;
    x=21;
    unsigned char *buf = (unsigned char *) calloc(STRLEN, sizeof(unsigned char));
    if (!buf)exit(50);
    if (!(fp = fopen("m2test.png", "rb"))) {
        printf("file can't be open\n");
        exit(100);
    }
    while (((size = fread(buf, sizeof(char), STRLEN, fp)) > 0)&&x>1) {
        mvwprintw(main_win,cur_str,1,"%08X: ", fx);
        for (int i = 0; i < size; i++) {
            if (buf[i] > 255)buf[i] -= 255;
            mvwprintw(main_win,cur_str,i*3+12,"%02X ", buf[i]);
        }
        if (size < STRLEN) {
            unsigned long indent = size;
            indent -= STRLEN;
            indent *= -1;
            for (int i = 0; i < indent; i++) {
                wprintw(main_win,"   ");
            }
        }
        for (int i = 0; i < size; i++) {
            if (buf[i] < 32 || buf[i] > '~' || buf[i] == '\n') {
                mvwprintw(main_win,cur_str,i+STRLEN*4,".");
            } else
                mvwprintw(main_win,cur_str,i+STRLEN*4,"%c", buf[i]);
        }
        cur_str++;
        fx += STRLEN;
        x--;
    }
}
