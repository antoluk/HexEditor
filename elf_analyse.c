//
// Created by antoluk on 7.5.23.
//

#include "elf_analyse.h"
Elf32_Ehdr_s read_elf()
{
    Elf32_Ehdr_s ehdr;
    FILE *fp;
    if (!(fp=fopen(filepath, "rb"))) {
        fprintf(stderr, "cant open file\n");
        exit(1);
    }
    fread(&ehdr, sizeof(ehdr), 1, fp);
    return ehdr;
}