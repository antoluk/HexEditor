//
// Created by antoluk on 13.5.23.
//

#ifndef HEXEDITORSECONDTRY_ANALYSE_H
#define HEXEDITORSECONDTRY_ANALYSE_H
#include "../headers/defines.h"
#include "../headers/signatures.h"
#include "../headers/png_analyse.h"
#include "../headers/elf_analyse.h"

extern int file_type;
extern const unsigned char* ext;
extern IHDR_t IHDR;
extern Elf32_Ehdr_s ehdr;
void analyse();
#endif //HEXEDITORSECONDTRY_ANALYSE_H
