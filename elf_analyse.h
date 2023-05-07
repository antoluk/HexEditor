//
// Created by antoluk on 7.5.23.
//

#ifndef HEXEDITORSECONDTRY_ELF_ANALYSE_H
#define HEXEDITORSECONDTRY_ELF_ANALYSE_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
typedef struct {
    char magic[4];
    char class;
    char encoding;
    char version;
    char osabi;
    char abiversion;
    char padding[7];
    short type;
    short machine;
    int version2;
} Elf32_Ehdr_s;
extern char* filename;
Elf32_Ehdr_s read_elf();
#endif //HEXEDITORSECONDTRY_ELF_ANALYSE_H
