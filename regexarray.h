/* header for csv_tools */

#ifndef REGEXARRAY
#define REGEXARRAY

#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <errno.h>
#include <string.h>

enum { STRING = 0x00, FLOAT = 0x01, TIME = 0x02, DATE= 0x04, PERCENT= 0x08, LONG=0x10, NIL=0x20 };

/* regular expression vector */
typedef struct
{
   unsigned char lgreg_list;
   regex_t *reg_list;
} regexarray;

/* prototypes */

regexarray *reg_init (void);
void free_reg (regexarray * rp);
int typedata (regexarray * p, const char *strtest);

#endif
