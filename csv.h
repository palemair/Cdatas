/* header for csv_tools */

#ifndef CSV_TOOLS
#define CSV_TOOLS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include "table.h"
#include "regexarray.h"
#include "xtools.h"

/* globals */
enum { STRING = 0x00, FLOAT = 0x01, TIME = 0x02, DATE= 0x04, PERCENT= 0x08, LONG=0x10, NIL=0x20 };

/* prototypes */

int append (struct list **ls, regexarray * rg, void *value);
int typedata (regexarray * p, const char *strtest);
int assign (struct field *fd, const char *value);
int try_assign(struct field* f,void* value,regexarray* rg);

struct table* load_csv (char *filename, char delim, bool header);
int parse_csv (char *datas, regexarray * rp, struct table **tb);

#endif
