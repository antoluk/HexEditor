//
// Created by antoluk on 5.4.23.
//
#include "event_handler.h"
#include "stack.h"

int letter = 1;


int event() {
    if (kbhit()) {
        fflush(stdin);
        int key = getch();

        switch (key) {
            case KEY_DOWN:
                move_down();
                break;
            case KEY_UP:
                move_up();
                break;
            case KEY_LEFT: {
                move_left();
                break;
            }
            case KEY_RIGHT:
                move_right();
                break;
            case KEY_F(5):
                pop(&stack);
                if (letter == 2 && change_mod == ASCII) {
                    cur.x--;
                    letter = 1;
                }
                return -3;
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
                exit_win();
                return -1;
            default:
                if (change_mod == HEX) {
                    if (!((key >= 48 && key <= 57) || (key >= 65 && key <= 70) || (key >= 97 && key <= 102)))
                        return -2;
                }
                if (change_mod == ASCII) {
                    if (!(key >= 32 && key <= 126))
                        return -2;
                }
                return key;
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


void move_left() {
    if (cur.x == 12 && inFile.y + inFile.x > 0) {
        int turns;
        if (change_mod == HEX) {
            turns = 31;
        } else turns = 15;
        move_up();
        for (int i = 0; i < turns; i++)move_right();
        return;
    }
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
}

void move_right() {
    if (cur.x == 58 || (cur.x == 57 && change_mod == ASCII)) {
        int turns;
        if (change_mod == HEX) {
            turns = 31;
        } else turns = 15;
        move_down();
        for (int i = 0; i < turns; i++)move_left();
        if (file_size <= inFile.x + inFile.y) { move_left(); }
        return;
    }
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
        else {
            if (cur.x == 58) {
                cur.x--;
                letter = 1;
            }
        }

    }
}

void move_up() {
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

void move_down() {
    if (cur.y < 20) {
        cur.y++;
        ccur.y++;
    } else dump_offset += 16;
    inFile.y += 16;
}

