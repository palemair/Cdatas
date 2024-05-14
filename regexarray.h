/* header for csv_tools */

#ifndef REGEXARRAY
#define REGEXARRAY

#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <errno.h>
#include "xtools.h"

/* regular expression vector */
typedef struct
{
   unsigned char lgreg_list;
   regex_t *reg_list;
} regexarray;

/* prototypes */

regexarray *reg_init (void);
void free_reg (regexarray * rp);

#endif
