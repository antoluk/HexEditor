#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "signatures.h"
#include <ncurses.h>
#include "png_analyse.h"
#include "event_handler.h"
#include "menu.h"
#include "globals.h"
#include "stack.h"
#include "file_work.h"


change change_mod = HEX;

Point cur, ccur, inFile;

struct node *stack;

void print_with_color(Point p, unsigned char c);

void win_init();

unsigned char print_dump(int str_num);

void printIHDR(struct IHDR IHDR);


int main() {
    struct IHDR IHDR;
    int key = -2;
    struct sign signatures[50];
    bool flags[10];
    signatures_init(signatures);
    const unsigned char *ext = signcheck();
    unsigned char cur_ch;
    int old_offset = 0, str_num = 0, changed = 0;
    if (!strncmp("png", (char *) ext, 3)) {
        IHDR = png_analyse();
        flags[0] = 1;
    }
    win_init();
    int screen_x = 0, screen_y = 0;
    start_color();
    keypad(stdscr, TRUE);
    init_pair(STYLE_CURSOR, COLOR_BLACK, COLOR_WHITE);

    inFile.x = 0;
    inFile.y = 0;
    cur.x = 12;
    cur.y = 1;
    ccur.x = cur.x + 52;
    ccur.y = cur.y;
    str_num = get_dump();
    while (true) {

        getmaxyx(stdscr, screen_y, screen_x);
        if (screen_y < 28 || screen_x < 84) {

            endwin();
            printf("min terminal size 84x28");
            return 1;
        }
        wclear(analyse_win);
        wclear(main_win);
        wclear(stdscr);
        box(main_win, 0, 0);
        box(analyse_win, 0, 0);
        if (old_offset != dump_offset || changed) {
            str_num = get_dump();
            old_offset = dump_offset;
            changed = 0;
        }
        if (inFile.y + inFile.x > file_size - 1) {
            inFile.y -= 16;
            cur.y -= 1;
            ccur.y -= 1;
        }
        cur_ch = print_dump(str_num);

        if (flags[0] == 1) {
            printIHDR(IHDR);
        }
        print_with_color(ccur, cur_ch);
        render_menu();
        wmove(main_win, cur.y, cur.x);
        refresh();
        wrefresh(analyse_win);
        wrefresh(main_win);
        flushinp();
        fflush(stdin);
        key = event();
        if (key == -1) {
            break;
        } else if (key > 0) {
            changed = change_file(key);
            key = -2;
        } else if (key == -3) {
            changed = 1;
        }

    }
    endwin();

    return 0;
}

void win_init() {
    initscr();
    cbreak();
    noecho();
    analyse_win = newwin(5, 82, 22, 1);
    main_win = newwin(22, 82, 0, 1);
}

unsigned char print_dump(int str_num) {
    unsigned char cur_ch;
    unsigned long size = 16;
    int fx = dump_offset, cur_str = 1;
    mvwprintw(main_win, 0, 1, "%08X", inFile.y + inFile.x);
    for (int j = 0; j < str_num; j++) {
        if (j == str_num - 1 && last_size < 16) {
            size = last_size;
        }
        if (size > 0) {
            mvwprintw(main_win, cur_str, 1, "%08X: ", fx);
            for (int i = 0; i < (int) size; i++) {
                if (bufer[j][i] > 255)bufer[j][i] -= 255;
                mvwprintw(main_win, cur_str, i * 3 + 12, "%02X ", bufer[j][i]);
            }
            if (size < STRLEN) {
                unsigned long indent = size;
                indent -= STRLEN;
                indent *= -1;
                for (int i = 0; i < indent; i++) {
                    wprintw(main_win, "   ");
                }
            }
            for (int i = 0; i < size; i++) {
                if (bufer[j][i] < 32 || bufer[j][i] > '~' || bufer[j][i] == '\n') {
                    mvwprintw(main_win, cur_str, i + STRLEN * 4, ".");
                    if (cur.y == cur_str && inFile.x == i)cur_ch = '.';
                } else {
                    mvwprintw(main_win, cur_str, i + STRLEN * 4, "%c", bufer[j][i]);
                    if (cur.y == cur_str && inFile.x == i)cur_ch = bufer[j][i];
                }
            }
            cur_str++;
            fx += STRLEN;
        }
    }
    return cur_ch;
}

void printIHDR(struct IHDR IHDR) {
    int str_num = 0;
    mvwprintw(analyse_win, 1, 1, "file extension is png ");
    wprintw(analyse_win, "%ld x %ld, ", IHDR.x, IHDR.y);
    wprintw(analyse_win, "%ld-bit", IHDR.bit_depth);
    switch (IHDR.color) {
        case 0:
            wprintw(analyse_win, " monochrome");
            break;
        case 2:
            wprintw(analyse_win, "/color RGB");
            break;
        case 3:
            wprintw(analyse_win, " colormap");
            break;
        case 4:
            wprintw(analyse_win, " Each pixel is a grayscale sample, followed by an alpha sample");
            break;
        case 6:
            wprintw(analyse_win, "/color RGBA");
            break;
        default:
            wprintw(analyse_win, " unknown");
    }
    if (IHDR.interplaced == 0) {
        wprintw(analyse_win, ", non-interlaced");
    } else if (IHDR.interplaced == 1) {
        wprintw(analyse_win, ", interlaced");
    } else wprintw(analyse_win, ", bad parameter");
}

void print_with_color(Point p, unsigned char c) {
    wattron(main_win, COLOR_PAIR(STYLE_CURSOR));
    mvwprintw(main_win, p.y, p.x, "%c", c);
    wattroff(main_win, COLOR_PAIR(STYLE_CURSOR));
}