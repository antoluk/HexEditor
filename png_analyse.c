//
// Created by antoluk on 12.3.23.
//

#include "png_analyse.h"

#define U2C (char*)


struct IHDR png_analyse() {
    FILE *fp;
    int chunk_size = 0;
    if (!(fp = fopen("m2test.png", "rb"))) {
        printf("file can't be open\n");
        exit(100);
    }
    unsigned char *buf = (unsigned char *) calloc(8, sizeof(unsigned char));
    if (!buf)exit(50);
    fseek(fp, PNG_sign_size, 0);
    int i = 0;
    fread(buf, sizeof(char), 4, fp);
    for (; i < 4; i++) {
        chunk_size += (int) buf[i];
    }
    fread(buf, sizeof(char), 4, fp);
    if (strcmp(U2C buf, "IHDR") == 0) {

        unsigned char *chunk = (unsigned char *) calloc(chunk_size + 1, sizeof(unsigned char));
        fread(chunk, sizeof(unsigned char), chunk_size + 1, fp);

       return IHDR_analyse(chunk);
    }
    struct IHDR fail;
    fail.x=0;
    fail.y=0;
    return fail;
}



struct IHDR IHDR_analyse(unsigned char *chunk_buf) {
    struct IHDR IHDR;
    unsigned char resY_str[8] = "";
    unsigned char resX_str[8] = "";
    unsigned char buf[4];
    unsigned char byte_str[2];
    int byte;
    int i = 0;
    for (; i < 4; i++) {
        sprintf(U2C buf, "%02X", chunk_buf[i]);
        strcat(U2C resX_str, U2C buf);
    }
    IHDR.x = strtol(U2C resX_str, NULL, 16);
    buf[0] = '\0';
    for (; i < 8; i++) {
        sprintf(U2C buf, "%02X", chunk_buf[i]);
        strcat(U2C resY_str, U2C buf);
    }
    IHDR.y = strtol(U2C resY_str, NULL, 16);
    sprintf(U2C byte_str, "%0X", chunk_buf[8]);
    IHDR.bit_depth = strtol(U2C byte_str, NULL, 16);//bit depth
    sprintf(U2C byte_str, "%0X", chunk_buf[9]);//color type
    IHDR.color = strtol(U2C byte_str, NULL, 16);
    sprintf(U2C byte_str, "%0X", chunk_buf[10]);//color type
    IHDR.interplaced = strtol(U2C byte_str, NULL, 16);
    return IHDR;
}
