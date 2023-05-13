//
// Created by antoluk on 3.5.23.
//

#include "../headers/file_work.h"

int get_dump() {
    if (!(fp = fopen(filepath, "rb"))) {
        printf("file can't be open\n");
        endwin();
        exit(100);
    }
    fseek(fp, 0, 2);
    file_size = (int) ftell(fp);
    fseek(fp, dump_offset, 0);
    for (int j = 0; j < 20; j++) {

        if ((last_size = (int) fread(bufer[j], sizeof(char), STRLEN, fp)) < 16) {
            return j + 1;
        }

    }
    fclose(fp);
    return 20;
}

int change_file(int symbol) {
    int read_from_file = 0;
    if (!(fp = fopen(filepath, "rb+"))) {
        printf("file can't be open\n");
        endwin();
        exit(-1);
    }
    fseek(fp, inFile.y + inFile.x, 0);
    fscanf(fp, "%c", &read_from_file);
    fseek(fp, -1, 1);
    if (change_mod == HEX) {
        char hex[3];
        sprintf(hex, "%02X", read_from_file);
        if(letter==2) {
            hex[1] = (char) symbol;
        } else hex[0]=(char)symbol;
        symbol = (int) strtol(hex, NULL, 16);
    }
    push(&stack, read_from_file);
    fwrite(&symbol, 1, 1, fp);
    fclose(fp);
    move_right();
    return 1;
}
