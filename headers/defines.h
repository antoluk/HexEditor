//
// Created by antoluk on 13.5.23.
//

#ifndef HEXEDITOR_DEFINES_H
#define HEXEDITOR_DEFINES_H
#define STRLEN 16
#define STYLE_CURSOR 14
#define PNG 1
#define ELF 2
#define BIN 3
#define PDB 4
#define DBA 5
#define ICO 6
#define TAR_LZW 7
#define TAR_LZH 8
#define EXE 9
#define MP3 10

typedef struct {
    char magic[4];
    char f_class;
    char encoding;
    char version;
    char osabi;
    char abiversion;
    char padding[7];
    short type;
    short machine;
    int version2;
} Elf32_Ehdr_s;

typedef struct {
    long x;
    long y;
    long bit_depth;
    long color;
    long interplaced;
}IHDR_t;
#endif //HEXEDITOR_DEFINES_H
