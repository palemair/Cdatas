/* handle a csvfile with C */
#define _GNU_SOURCE
#include "csv.h"

struct list* init_list (void)
{
    struct list* new = xmalloc (sizeof (*new));

    new->len = 0;
    new->head = NULL;
    new->tail = NULL;

    return new;
}

struct table* init_table (char delim, bool header)
{
    struct table* new = xmalloc (sizeof (*new));

    new->Dlim = delim;
    new->header = header;
    new->width = 0;
    new->height = 0;
    new->t = NULL;

    return new;
}

/* void del_field_by_index (struct list* ls,uint16_t index) */
/* { */
/*     struct field* curr = ls->head; */
/*     struct field* tmp; */
/*     uint8_t count = 0; */
/*     while (curr && count++ != index) curr = curr->nxt; */

/*     tmp = curr; */
/*     curr->prv->nxt = curr->nxt; */
/*     curr->nxt->prv = curr->prv; */

/*     if (tmp->datatype & STRING) */
/*         { */
/*             free (tmp->strdata); */
/*         } */
/*         free (tmp); */
/* } */

void del_list (struct list* ls)
{
    if(ls !=NULL)
    {
        struct field* tmp;
        struct field* pfd = ls->head;
        while (pfd)
        {
            tmp = pfd;
            if (tmp->datatype & STRING)
            {
                free (tmp->strdata);
            }
            pfd = pfd->nxt;
            free (tmp);
        }
        ls->head = NULL;
        ls->tail = NULL;
    }
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

int append (struct list** ls, regexarray* rg, void* value)
{
    int Error_assign = -1;

    if (*ls == NULL)
    {
        fprintf (stderr, "%s\n", "Init struct list first !!");
    }
    else
    {
        struct field* new = xmalloc (sizeof (*new));
        Error_assign = try_assign (new, value, rg);

        if (Error_assign == -1)
        {
            fprintf (stderr, "%s\n", "assignement error !!");
            return Error_assign;
        }

        if ((*ls)->tail == NULL)
        {
            new->prv = (*ls)->tail;
            (*ls)->tail = new;
            (*ls)->head = new;
        }
        else
        {
            new->prv = (*ls)->tail;
            (*ls)->tail->nxt = new;
            (*ls)->tail = new;
        }
        (*ls)->len++;
    }
    return Error_assign;
}
