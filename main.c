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
    struct table* tb = load_csv ("CSV-file/technic.csv", ',', true);
    STOP;
    
    printf("nb de colonnes : %d\n", tb->width);
    printf("nb de lignes : %d\n", tb->height);

    head(tb);
    tail(tb);

    drop_table (tb);

    TPS ("load_csv");
    return EXIT_SUCCESS;
}
