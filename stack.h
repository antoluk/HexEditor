//
// Created by antoluk on 3.5.23.
//

#ifndef HEXEDITORSECONDTRY_STACK_H
#define HEXEDITORSECONDTRY_STACK_H

#include "event_handler.h"

extern FILE *fp;
struct node {
    int data;
    Point s_cur;
    Point s_in_file;
    Point s_curr;
    int s_letter;
    int s_dump_offset;
    struct node *next;
};

void push(struct node **, int data);

void pop(struct node **);

#endif //HEXEDITORSECONDTRY_STACK_H
