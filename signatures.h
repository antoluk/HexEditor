//
// Created by antoluk on 12.3.23.
//

#ifndef HEXEDITORSECONDTRY_SIGNATURES_H
#define HEXEDITORSECONDTRY_SIGNATURES_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct sign {
    char file_extention[100];
    char file_signature[100];
};
extern char* filepath;
int signatures_init(struct sign *signatures);

unsigned char *signcheck();

#endif //HEXEDITORSECONDTRY_SIGNATURES_H
