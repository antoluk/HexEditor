//
// Created by antoluk on 3.5.23.
//

#include "stack.h"

void push(struct node **stack, int data) {
    struct node *node = (struct node *) calloc(1, sizeof(struct node));
    node->data = data;
    node->s_cur = cur;
    node->s_curr = ccur;
    node->s_in_file = inFile;
    node->s_letter = letter;
    node->s_dump_offset = dump_offset;
    node->next = (*stack);
    (*stack) = node;
}

void pop(struct node **stack) {
    struct node *node = (*stack);

    if (node) {
        int symbol = node->data;
        cur = node->s_cur;
        ccur = node->s_curr;
        inFile = node->s_in_file;
        letter = node->s_letter;
        dump_offset = node->s_dump_offset;
        (*stack) = (*stack)->next;
        free(node);
        if (!(fp = fopen(filepath, "rb+"))) {
            printf("file can't be open\n");
            exit(100);
        }
        fseek(fp, inFile.y + inFile.x, 0);
        fwrite(&symbol, 1, 1, fp);
        fclose(fp);
    }

}