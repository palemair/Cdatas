/* Handle a csvfile with C */
#define _GNU_SOURCE
#include <time.h>
#include "csv.h"
#include "print.h"
#include "xtools.h"
#include "iter.h"

int main ()
{
    START;
    struct table* tb = load_csv ("CSV-file/technic.csv", ',', true);
    STOP;
    
    listiter il = xmalloc(sizeof(*il));
    tabiter it = xmalloc(sizeof(*it));
    init_tabiter(it, tb);
    
    while(next_list(it))
    {
        init_listiter(il, it->curr);
        while(next_field(il)) print_field(il->curr,10,25);
        printf("\n");
    }

    free(il);
    free(it);
    drop_table (tb);
    
    TPS ("load_csv");
    return EXIT_SUCCESS;
}
