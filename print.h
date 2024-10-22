/* print tabula datas from csvfile with C */
#ifndef PRINT
#define PRINT

#include "regexarray.h"
#include <stdint.h>

uint16_t* mkwidth(struct table* tb);

void lineH(struct table* tb, int lg,uint16_t* wd);

int print_header(struct table* tb);

void printline (struct table* tb, uint32_t index);

void printlines(struct table* tb, uint32_t start, uint32_t stop);

void printable(struct table* tb);

void r_printable(struct table* tb, unsigned int step);

int head(struct table* tb, unsigned int step);

int tail(struct table* tb, unsigned int step);

#endif
