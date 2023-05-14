//
// Created by antoluk on 12.3.23.
//

#ifndef HEXEDITOR_PNG_ANALYSE_H
#define HEXEDITOR_PNG_ANALYSE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "../headers/defines.h"

#define PNG_sign_size 8

extern char* filepath;


IHDR_t png_analyse();

IHDR_t IHDR_analyse(unsigned char *chunk_buf);

#endif //HEXEDITOR_PNG_ANALYSE_H
