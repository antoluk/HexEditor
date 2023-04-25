//
// Created by antoluk on 25.4.23.
//

#ifndef HEXEDITORSECONDTRY_GLOBALS_H
#define HEXEDITORSECONDTRY_GLOBALS_H

WINDOW *main_win = NULL, *analyse_win = NULL;
#define STRLEN 16
#define STYLE_CURSOR 14
int dump_offset = 0;
extern int letter;
unsigned char bufer[20][16];
int last_size = 0, file_size = 0;

FILE *fp;

#endif //HEXEDITORSECONDTRY_GLOBALS_H
