/* Handle a csvfile with C */
#define _GNU_SOURCE
#include <time.h>
#include "csv.h"
#include "print.h"
#include "xtools.h"
#include "iter.h"

int main ()
{
    struct table* tb = load_csv ("CSV-file/xaa", ',', true);
    
    START;
    
    listiter il = xmalloc(sizeof(*il));
    unsigned int i = 0;
    while(i<tb->height)
    {
        init_listiter(il, tb->t[i]);
        while(next_field(il)) print_field(il->curr,tb->fmtnum,tb->fmtstr,tb->fmtprecis);
        printf("\n");
        i++;
    }

    free(il);
    STOP;
    
    (void)write_csv("testw.csv",tb);

    drop_table (tb);
    
    TPS ("load_csv");
    return EXIT_SUCCESS;
}
