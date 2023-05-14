//
// Created by antoluk on 3.5.23.
//

#ifndef HEXEDITOR_FILE_WORK_H
#define HEXEDITOR_FILE_WORK_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "event_handler.h"
#include "stack.h"

#define STRLEN 16

extern unsigned char bufer[20][16];
extern int last_size;
int change_file(int symbol);
int get_dump();

#endif //HEXEDITOR_FILE_WORK_H
