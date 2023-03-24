#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "signatures.h"
#include "analyse.h"
#include <ncurses.h>
#include "graphic.h"



int main() {
    struct sign signatures[50];
    int sign_size= signatures_init(signatures);

    win_init();
 //   analyse(signcheck());
 //   printw("%d, %d",x,y);

    return 0;
}
