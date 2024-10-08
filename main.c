/* Handle a csvfile with C */
#define _GNU_SOURCE
#include <time.h>
#include <locale.h>
#include <wchar.h>
#include "csv.h"
#include "print.h"
#include "xtools.h"
#include "iter.h"

int main ()
{
    if(setlocale(LC_CTYPE | LC_COLLATE,"") == NULL)
    {
        perror("setlocale");
        return EXIT_FAILURE;
    }

    START;
    struct table* tb = load_csv ("CSV-file/xaa", ',', true,true);
    STOP;
    
    TPS ("load_csv");

    r_printable(tb, 5);

    /* for(int i = 0;i<880; i++ ) */
    /* { */
    /*     u = next_iter(iter); */
    /*     if(u == 2) */
    /*     { */
    /*         putchar('\n'); */
    /*         printf("| %5ld - ",iter->ypos); */
    /*     } */
    /*     if(u == 0) */
    /*         break; */
    /*     print_iter(iter); */
    /* } */
    /* putchar('\n'); */

    /* print(tb,2,0,10); */
    /* head(tb); */
    /* tail(tb); */

    /* listiter il = xmalloc(sizeof(*il)); */
    /* unsigned int i = 1; */
    /* while(i<tb->height) */
    /* { */
    /*     init_listiter(il, tb->t[i]); */
    /*     while(next_field(il)) print_field(il->curr,tb->fmtnum,tb->fmtstr,tb->fmtprecis); */
    /*     printf("\n"); */
    /*     i++; */
    /* } */

    /* free(il); */
    
    /* write_csv("test",tb); */

    drop_table (tb);
    
    return EXIT_SUCCESS;
}
