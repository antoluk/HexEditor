//
// Created by antoluk on 7.5.23.
//

#ifndef HEXEDITOR_ELF_ANALYSE_H
#define HEXEDITOR_ELF_ANALYSE_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "../headers/defines.h"

extern char* filepath;
Elf32_Ehdr_s read_elf();
#endif //HEXEDITORSECONDTRY_ELF_ANALYSE_H
