//
// Created by antoluk on 12.3.23.
//
#include "signatures.h"

#define U2C (char*)

int signatures_init(struct sign *signatures) {
    FILE *Signatures_file = fopen("signatures.txt", "r");
    if (!Signatures_file) {
        printf("File with signatures cannot be open");
        exit(1);
    }
    int sign_arr_size = 0;
    while (1) {
        if (fgets(signatures[sign_arr_size].file_extention, 100, Signatures_file) == NULL)break;
        fgets(signatures[sign_arr_size].file_signature, 100, Signatures_file);
        sign_arr_size++;
    }
    fclose(Signatures_file);
    return sign_arr_size;
}

unsigned char *signcheck() {
    FILE *fp;
    unsigned char *extension = (unsigned char *) calloc(100, sizeof(unsigned char));
    struct sign signatures[50];
    int sign_arr_size = signatures_init(signatures);
    unsigned long size;
    unsigned char buf[100];
    unsigned char signbuf[8];
    fp = fopen(filepath, "rb");
    if (!fp) {
        printf("file can't be open\n");
        exit(1);
    }
    for (int i = 0; i < sign_arr_size; i++) {
        strcpy(U2C extension, "");
        size = fread(buf, sizeof(char), strlen(signatures[i].file_signature) / 2, fp);
        for (long unsigned int j = 0; j < size; j++) {
           // if (buf[j] > 255)buf[j] -= 255;
            sprintf(U2C signbuf, "%02X", buf[j]);
            strcat(U2C extension, U2C signbuf);
        }
        rewind(fp);
        if (!strncmp(U2C extension, signatures[i].file_signature, strlen(signatures[i].file_signature) - 1)) {
            strcpy(U2C extension, signatures[i].file_extention);
            break;
        }
        strcpy(U2C extension, "unknown");
    }
    fclose(fp);
    return extension;
}
