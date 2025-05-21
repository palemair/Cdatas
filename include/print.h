/* print tabula datas from csvfile with C */
#ifndef PRINT
#define PRINT

#include "regexarray.h"
#include <stdint.h>

/* static int lineH(struct table* tb); */

int print_header(struct table* tb);

int printline (struct table* tb, uint32_t index);

int printlines(struct table* tb, uint32_t start, uint32_t stop);

int printable(struct table* tb);

int r_printable(struct table* tb, unsigned int step);

int head(struct table* tb, unsigned int step);

int tail(struct table* tb, unsigned int step);

#endif
