/* header for csv_tools */

#ifndef TABLE
#define TABLE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "list.h"

/* table structure containing csv */
struct table
{
   struct list **t;
   char Dlim;
   bool header;
   uint16_t width;
   uint32_t height;
   uint8_t fdwidth;
   uint8_t fdprecis;
   size_t rowsremain;
};

/* prototypes */

struct table* init_table (char delim, bool header, int fdwidth);
void drop_table (struct table* tb);

#endif
