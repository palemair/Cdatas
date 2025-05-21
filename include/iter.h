#ifndef _ITER
#define _ITER

#include "table.h"

typedef struct Iter
{
    struct table *tb; //table
    struct field *curr; //current field
    ssize_t xpos;
    ssize_t ypos;
} iter;

typedef iter* iterator;

iterator init_iter(struct table *);
void destroy_iter(iterator it);
uint8_t next_iter(iterator it);

#endif
