/* Handle a csvfile with C */
#define _GNU_SOURCE
#include <time.h>
#include "csv.h"
#include "print.h"
#include "xtools.h"

/* struct table* transpose(struct table*tb) */
/* { */
    
/*     return NULL; */
/* } */

int main ()
{
    START;
    struct table* tb = load_csv ("CSV-file/xxx", ',', true);
    STOP;
    
    printall(tb);
    /* tail(tb); */

    drop_table (tb);
    
    TPS ("load_csv");
    return EXIT_SUCCESS;
}
