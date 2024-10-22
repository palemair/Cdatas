/* handle a csvfile with C */
/* #define _GNU_SOURCE */
#include "table.h"
#include "iter.h"
#include "xtools.h"

struct table* init_table (char delim, bool header, int fdwidth)
{
    struct table* new = xmalloc (sizeof (*new));

    new->Dlim = delim;
    new->header = header;
    new->width = 0;
    new->height = 0;
    new->desc = NULL;
    new->t = NULL;
    new->fdwidth = fdwidth;
    new->fdprecis = 2;
    new->rowsremain = 0;

    return new;
}

void drop_table (struct table* tb)
{
    del_list (tb->desc);
    free (tb->desc);
    for (int u = 0; u < (int) tb->height; u++)
    {
        del_list (tb->t[u]);
        free (tb->t[u]);
    }
    free (tb->t);
    free (tb);
}
