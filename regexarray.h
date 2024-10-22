/* header for csv_tools */

#ifndef REGEXARRAY
#define REGEXARRAY

#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <errno.h>
#include <string.h>

#define TYPE 0xFF
#define MAXWD 0xFF00

enum { STRING = 0x01, FLOAT = 0x02, TIME = 0x04,
       DATE= 0x08, PERCENT= 0x10, LONG=0x20, 
       NIL=0x40, DESC = 0x80, ERR=0x00 };

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
