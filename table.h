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
   char delim;
   bool header;
   uint16_t width;
   uint32_t height;
   struct list* desc;
   struct list** t;
   uint8_t fdprecis;
   uint32_t rowsremain;
   uint8_t lgcol1;
};

/* prototypes */

struct table* init_table (char delim, bool header);
void drop_table (struct table* tb);
void update_tb_fd_width(struct table* tb);

#endif
