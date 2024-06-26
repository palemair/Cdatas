/* print tabula datas from csvfile with C */
#ifndef PRINT
#define PRINT

#include "csv.h"
#include "regexarray.h"

int print_header(struct table *tb);
int print(struct table* tb, uint32_t start, uint32_t stop);
int printall(struct table* tb);
int head(struct table* tb);
int tail(struct table* tb);

#endif
