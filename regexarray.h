/* header for csv_tools */

#ifndef REGEXARRAY
#define REGEXARRAY

#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <errno.h>
#include <string.h>

enum { STRING = 0x01, FLOAT = 0x02, TIME = 0x04, DATE= 0x08, PERCENT= 0x16, LONG=0x32, NIL=0x64, ERR=0x1000 };

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
