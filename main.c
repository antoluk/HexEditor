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
int dump_offset=0;
extern int letter;
char bufer[22][82];

FILE *fp;
void print_with_color(Point p, unsigned char c);

void win_init();

unsigned char print_dump(Point cur);

void printIHDR(struct IHDR IHDR);

void change_file(int symbol);

void get_dump();

int main() {
    struct IHDR IHDR;
    int key=-2;
    struct sign signatures[50];
    bool flags[10];
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
        key=event();
        if(key==-1)
        {
            break;
        } else if(key>0)
        {
            change_file(key);
            key=-2;
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

unsigned char print_dump(Point cur) {
    //if(inFile.y>file_stat.st_size-19*16)inFile.y-=16;
    unsigned long size;
    int fx = dump_offset, x = 21, cur_str = 1;
    unsigned char *buf = (unsigned char *) calloc(STRLEN, sizeof(unsigned char));
    unsigned char cur_ch;
    if (!buf)exit(50);
    if (!(fp = fopen("cringe.txt", "rb"))) {
        printf("file can't be open\n");
        exit(100);
    }
    fseek(fp,0,2);
    if(inFile.y> ftell(fp))
    {
        inFile.y-=16;
        dump_offset-=16;

    }
    fseek(fp,dump_offset,0);
    mvwprintw(main_win, 0, 1, "%08X", inFile.y+inFile.x);
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

void change_file(int symbol)
{
    int read_from_file=0;
    if (!(fp = fopen("cringe.txt", "rb+"))) {
        printf("file can't be open\n");
        exit(100);
    }
    char hex[3];
    fseek(fp,inFile.y+inFile.x,0);
    fscanf(fp,"%c",&read_from_file);
    sprintf(hex,"%02X",read_from_file);
    fseek(fp,-1,1);
    printf(" %s ",hex);
    hex[letter-1]=symbol;
    symbol=strtol(hex,NULL,16);
    //printf(" %d ",symbol);
    int read=0;
  fwrite(&symbol,1,1,fp);

    fclose(fp);
}

void get_dump()
{
    unsigned long size;
    int fx = dump_offset, x = 21, cur_str = 1;
    if (!(fp = fopen("cringe.txt", "rb"))) {
        printf("file can't be open\n");
        exit(100);
    }
    fseek(fp,0,2);
    if(inFile.y> ftell(fp))
    {
        inFile.y-=16;
        dump_offset-=16;
    }
    fseek(fp,dump_offset,0);
    int j=0;
    for(int i=0;i<20;i++) {
        if (fread(bufer[j++], sizeof(char), STRLEN, fp) > 0)break;
    }
    fclose(fp);
}
