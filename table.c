/* handle a csvfile with C */
/* #define _GNU_SOURCE */
#include "table.h"
#include "iter.h"
#include "xtools.h"

struct table* init_table (char delim, bool header)
{
    struct table* new = xmalloc (sizeof (*new));

    new->Dlim = delim;
    new->header = header;
    new->width = 0;
    new->height = 0;
    new->t = NULL;
    new->fmtnum = 10;
    new->fmtstr = 3 * new->fmtnum;
    new->fmtprecis = 2;

    return new;
}

void drop_table (struct table* tb)
{
    for (int u = 0; u < (int) tb->height; u++)
    {
        del_list (tb->t[u]);
        free (tb->t[u]);
    }
    free (tb->t);
    free (tb);
}
