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
   struct list *first;
   uint16_t width;
   uint32_t height;
   uint8_t fmtstr;
   uint8_t fmtnum;
   uint8_t fmtprecis;
};

/* prototypes */

struct table *init_table (char delim, bool header);
void drop_table (struct table* tb);

#endif
