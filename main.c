#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "signatures.h"
#include <ncurses.h>
#include <unistd.h>
#include "png_analyse.h"
#include "event_handler.h"
#include <sys/stat.h>

WINDOW *main_win = NULL, *analyse_win = NULL;
#define STRLEN 16
#define STYLE_CURSOR 14
struct stat file_stat;
Point cur, ccur,inFile;
FILE *fp;
void print_with_color(Point p, unsigned char c);

void win_init();

unsigned char print_dump(Point cur);

void printIHDR(struct IHDR IHDR);

void running();

int main() {
    struct IHDR IHDR;

    struct sign signatures[50];
    bool flags[10];

    if (!(fp = fopen("m2test.png", "rb"))) {
        printf("file can't be open\n");
        exit(100);
    }
    fstat (fileno (fp), &file_stat);
    fclose(fp);
    int sign_size = signatures_init(signatures);
    const unsigned char *ext = signcheck();
    unsigned char cur_ch;

    if (!strncmp("png", (char *) ext, 3)) {
        IHDR = png_analyse();
        flags[0] = 1;
    }
    win_init();
    start_color();
    keypad(stdscr, TRUE);
    init_pair(STYLE_CURSOR, COLOR_BLACK, COLOR_WHITE);

    inFile.x=0;
    inFile.y=0;
    cur.x = 12;
    cur.y = 1;
    ccur.x = cur.x + 52;
    ccur.y = cur.y;
    while (true) {
        wclear(analyse_win);
        wclear(main_win);

        box(analyse_win, 0, 0);
        box(main_win, 0, 0);
        cur_ch = print_dump(ccur);

        if (flags[0] == 1) {
            printIHDR(IHDR);
        }
        print_with_color(ccur, cur_ch);
        wmove(main_win, cur.y, cur.x);
        refresh();
        wrefresh(analyse_win);
        wrefresh(main_win);
        flushinp();
        fflush(stdin);
        if(!event())break;
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

unsigned char print_dump(Point cur) {
    if(inFile.y>file_stat.st_size-19*16)inFile.y-=16;
    unsigned long size;
    int fx = inFile.y/16, x = 21, cur_str = 1;
    unsigned char *buf = (unsigned char *) calloc(STRLEN, sizeof(unsigned char));
    unsigned char cur_ch;
    if (!buf)exit(50);
    if (!(fp = fopen("m2test.png", "rb"))) {
        printf("file can't be open\n");
        exit(100);
    }
    fseek(fp,inFile.y,0);
    while (((size = fread(buf, sizeof(char), STRLEN, fp)) > 0) && x > 1) {
        mvwprintw(main_win, cur_str, 1, "%08X: ", fx);
        for (int i = 0; i < size; i++) {
            if (buf[i] > 255)buf[i] -= 255;
            mvwprintw(main_win, cur_str, i * 3 + 12, "%02X ", buf[i]);
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
            if (buf[i] < 32 || buf[i] > '~' || buf[i] == '\n') {
                mvwprintw(main_win, cur_str, i + STRLEN * 4, ".");
                if (cur.y == cur_str && cur.x-64 == i)cur_ch = '.';
            } else {
                mvwprintw(main_win, cur_str, i + STRLEN * 4, "%c", buf[i]);
                if (cur.y == cur_str && cur.x-64 == i)cur_ch = buf[i];
            }
        }
        cur_str++;
        fx += STRLEN;
        x--;
    }
    free(buf);
    fclose(fp);
    return cur_ch;
}

void printIHDR(struct IHDR IHDR) {
    int str_num = 0;
    mvwprintw(analyse_win, 1, 1, "%ld x %ld, ", IHDR.x, IHDR.y);
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
    mvwprintw(main_win, p.y, p.x,"%c", c);
    wattroff(main_win, COLOR_PAIR(STYLE_CURSOR));
}