/* Handle a csvfile with C */
#define _GNU_SOURCE
#include "csv.h"
#include "print.h"

/* struct table* transpose(struct table*tb) */
/* { */
    
/*     return NULL; */
/* } */

int main ()
{
    START;
    struct table* tb = load_csv ("CSV-file/xaa", ',', true);
    STOP;
    
    printall(tb);

    drop_table (tb);

    TPS ("load_csv");
    return EXIT_SUCCESS;
}
