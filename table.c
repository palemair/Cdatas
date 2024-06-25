/* handle a csvfile with C */
/* #define _GNU_SOURCE */
#include "table.h"
#include "iter.h"
#include "xtools.h"

enum { lgstr = 25, lgnumber = 10, lgprecision = 2};

struct table* init_table (char delim, bool header)
{
    struct table* new = xmalloc (sizeof (*new));

    new->Dlim = delim;
    new->header = header;
    new->width = 0;
    new->height = 0;
    new->t = NULL;
    new->fmtnum = lgnumber;
    new->fmtstr = 2.5 * lgnumber;
    new->fmtprecis = lgprecision;
    new->first = NULL;

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
