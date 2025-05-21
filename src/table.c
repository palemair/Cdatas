/* handle a csvfile with C */
/* #define _GNU_SOURCE */
#include "../include/table.h"
#include "../include/iter.h"
#include "../include/xtools.h"

struct table* init_table (char delim, bool header)
{
    struct table* new = xmalloc (sizeof (*new));

    new->delim = delim;
    new->header = header;
    new->width = 0;
    new->height = 0;
    new->desc = NULL;
    new->t = NULL;
    new->fdprecis = 2;
    new->rowsremain = 0;
    new->lgcol1 = 5;

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

void update_tb_fd_width (struct table* tb)
{
    char buff[12];
    int lg = sprintf (buff, "%d", tb->height);

    tb->lgcol1 = lg;

    uint16_t tmp[tb->width];
    uint16_t* p = tmp;
    struct field* tf = tb->desc->head;

    while (tf != NULL)
    {
        *p++ = tf->fdlenth;
        tf = tf->nxt;
    }

    iterator it = init_iter (tb);

    while (next_iter (it))
    {
        tmp[it->xpos] =
            (tmp[it->xpos] >= it->curr->fdlenth) ? tmp[it->xpos] : it->curr->fdlenth;
    }
    destroy_iter (it);

    tf = tb->desc->head;
    p = tmp;
    while (tf != NULL)
    {
        tf->fdlenth = *p++;
        tf = tf->nxt;
    }
}
