//
// Created by antoluk on 13.5.23.
//

#include "../headers/analyse.h"

void analyse() {
    ext = signcheck();
    if (!strncmp("png", (char *) ext, 3)) {
        IHDR = png_analyse();
        file_type = PNG;
    } else if (!strncmp("elf", (char *) ext, 3)) {
        ehdr = read_elf();
        file_type = ELF;
    } else if (!strncmp("bin", (char *) ext, 3)) {
        file_type = BIN;
    } else if (!strncmp("PDB", (char *) ext, 3)) {
        file_type = PDB;
    } else if (!strncmp("ico", (char *) ext, 3)) {
        file_type = ICO;
    } else if (!strncmp("tar.z LZW algorithm", (char *) ext, 19)) {
        file_type = TAR_LZW;
    } else if (!strncmp("tar.z LZH algorithm", (char *) ext, 19)) {
        file_type = TAR_LZH;
    } else if (!strncmp("exe", (char *) ext, 3)) {
        file_type = EXE;
    } else if (!strncmp("mp3", (char *) ext, 3)) {
        file_type = MP3;
    } else if (!strncmp("DBA", (char *) ext, 3)) {
        file_type = DBA;
    }
}