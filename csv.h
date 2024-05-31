/* header for csv_tools */

#ifndef CSV_TOOLS
#define CSV_TOOLS

#include <stdbool.h>
#include "table.h"
#include "regexarray.h"

/* prototypes */

int append_value (struct list **ls, regexarray * rg, void *value);
int assign (struct field *fd, const char *value,regexarray* rg);

struct table* load_csv (char *filename, char delim, bool header);
int parse_csv (char *datas, regexarray * rp, struct table **tb);

#endif
