//
// Created by antoluk on 5.4.23.
//
#include "event_handler.h"

int letter = 1;

int event() {
    if (kbhit()) {
        fflush(stdin);
        int key = getch();

        switch (key) {
            case KEY_DOWN:

                if (cur.y < 20) {
                    cur.y++;
                    ccur.y++;
                } else dump_offset += 16;
                inFile.y += 16;
                break;
            case KEY_UP: {
                if (cur.y > 1) {
                    cur.y--;
                    ccur.y--;
                } else if (dump_offset > 0) {
                    dump_offset -= 16;
                }
                if (inFile.y > 0) {
                    inFile.y -= 16;
                }

            }
                break;
            case KEY_LEFT: {
                for (int i = 0; i < 2; i++) {
                    if (cur.x > 12) {
                        if (letter == 1) {
                            ccur.x--;
                            cur.x--;
                            inFile.x--;
                            letter = 2;
                        } else letter = 1;
                        cur.x--;
                    }
                    if (change_mod == HEX)break;
                }
                break;
            }
            case KEY_RIGHT:
                for (int i = 0; i < 2; i++) {
                    if (cur.x < 58) {
                        if (letter == 2) {
                            ccur.x++;
                            cur.x++;
                            inFile.x++;
                            letter = 1;
                        } else letter = 2;
                        cur.x++;
                    }
                    if (change_mod == HEX)break;
                    else
                    {
                        if(cur.x==58) {
                            cur.x--;
                            letter=1;
                        }
                    }

                }
                break;

            case KEY_F(4):
                change_mod = HEX;
                break;
            case KEY_F(3):
                change_mod = ASCII;
                if (letter == 2) {
                    cur.x--;
                    letter = 1;
                }
                break;
            case KEY_F(2):
                go_to();
                break;
            case KEY_F(1):
                return -1;
            default:
                return key;

                break;
        }
    }
    return -2;
}

int kbhit() {
    int ch = getch();

    if (ch != ERR) {
        ungetch(ch);
        return 1;
    } else {
        return 0;
    }
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
    } while ((inp = getch()) != KEY_F(1));
    curs_set(1);
    delwin(go_to);
}
