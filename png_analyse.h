//
// Created by antoluk on 12.3.23.
//

#ifndef HEXEDITORSECONDTRY_PNG_ANALYSE_H
#define HEXEDITORSECONDTRY_PNG_ANALYSE_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#define PNG_sign_size 8

void png_analyse();
void IHDR_analyse(unsigned char* chunk_buf);
#endif //HEXEDITORSECONDTRY_PNG_ANALYSE_H
