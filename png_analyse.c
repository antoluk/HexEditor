//
// Created by antoluk on 12.3.23.
//

#include "png_analyse.h"
struct IHDR
{
    int x;
    int y;
    int bit_depth;
    int color;
    int interplaced
};

void png_analyse() {
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
    if (strcmp(buf, "IHDR") == 0) {

        unsigned char *chunk = (unsigned char *) calloc(chunk_size + 1, sizeof(unsigned char));
        fread(chunk, sizeof(unsigned char), chunk_size + 1, fp);

        IHDR_analyse(chunk);
    }
}

void printIHDR(struct IHDR IHDR)
{
    printf("%d x %d, ",IHDR.x,IHDR.y);
    printf("%d-bit",IHDR.bit_depth);
    switch (IHDR.color) {
        case 0:
            printf(" monochrome");
            break;
        case 2:
            printf("/color RGB");
            break;
        case 3:
            printf(" colormap");
            break;
        case 4:
            printf(" Each pixel is a grayscale sample, followed by an alpha sample");
            break;
        case 6:
            printf("/color RGBA");
            break;
        default:
            printf(" unknown");
    }
    if (IHDR.interplaced == 0) {
        printf(", non-interlaced");
    } else if (IHDR.interplaced == 1) {
        printf(", interlaced");
    } else printf(", bad parameter");
}

void IHDR_analyse(unsigned char *chunk_buf) {
    struct IHDR IHDR;
    unsigned char resY_str[8] = "";
    unsigned char resX_str[8] = "";
    unsigned char buf[4];
    unsigned char byte_str[2];
    int byte;
    int i = 0;
    for (; i < 4; i++) {
        sprintf(buf, "%02X", chunk_buf[i]);
        strcat(resX_str, buf);
    }
    IHDR.x = strtol(resX_str, NULL, 16);
    buf[0] = '\0';
    for (; i < 8; i++) {
        sprintf(buf, "%02X", chunk_buf[i]);
        strcat(resY_str, buf);
    }
    IHDR.y = strtol(resY_str, NULL, 16);
    sprintf(byte_str, "%02X", chunk_buf[8]);
    IHDR.bit_depth = strtol(byte_str, NULL, 16);//bit depth
    sprintf(byte_str, "%02X", chunk_buf[9]);//color type
    IHDR.color = strtol(byte_str, NULL, 16);
    sprintf(byte_str, "%02X", chunk_buf[10]);//color type
    IHDR.interplaced = strtol(byte_str, NULL, 16);
    printIHDR(IHDR);


}

