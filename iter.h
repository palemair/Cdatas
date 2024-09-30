#ifndef _ITER
#define _ITER

#include "table.h"

typedef struct Iterator
{
    struct table *tb; //table
    struct field *curr; //current field
    ssize_t xpos;
    ssize_t ypos;
} iterator;


void init_iter(iterator* it,struct table *);
uint8_t next_iter(iterator* it);
void fprint_iter(iterator* it,FILE* outputfile);
void print_iter(iterator * it);

#endif
