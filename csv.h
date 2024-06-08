/* header for csv_tools */

#ifndef CSV_TOOLS
#define CSV_TOOLS

#include <stdbool.h>
#include "table.h"
#include "regexarray.h"

/* prototypes */
struct table* load_csv (char *filename, char delim, bool header);
int parse_csv (char *datas, regexarray * rp, struct table **tb);

#endif
