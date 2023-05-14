//
// Created by antoluk on 25.4.23.
//

#ifndef HEXEDITOR_GLOBALS_H
#define HEXEDITOR_GLOBALS_H
#include "../headers/defines.h"

WINDOW *main_win = NULL, *analyse_win = NULL;
IHDR_t IHDR;
Elf32_Ehdr_s ehdr;
change change_mod = HEX;
Point cur, ccur, inFile;
int dump_offset = 0;
int letter=1;
unsigned char bufer[20][16];
int last_size = 0, file_size = 0;
char *filepath;
char *filename;
FILE *fp;
struct node *stack;
int file_type = 0;
const unsigned char *ext;

#endif //HEXEDITOR_GLOBALS_H
