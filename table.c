/* handle a csvfile with C */
#define _GNU_SOURCE
#include "table.h"
#include "xtools.h"

#define DELSTRING(x) if (x->datatype == 0x00) free (x->strdata)

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

struct list* init_list (void)
{
    struct list* new = xmalloc (sizeof (*new));

    new->len = 0;
    new->head = NULL;
    new->tail = NULL;

    return new;
}

void del_list (struct list* ls)
{
    if(ls !=NULL)
    {
        struct field* tmp;
        struct field* pfd = ls->head;
        while (pfd)
        {
            tmp = pfd;
            DELSTRING(tmp);
            pfd = pfd->nxt;
            free (tmp);
        }
        ls->head = NULL;
        ls->tail = NULL;
    }
}


void pop (struct list** ls)
{
    struct field* tmp;
    struct field* fd = (*ls)->tail;

    if (fd)
    {
        tmp = fd;
        if (fd->prv == NULL)
        {
            (*ls)->len = 0;
            (*ls)->head = NULL;
            (*ls)->tail = NULL;
        }
        else
        {
            tmp = fd;
            fd = fd->prv;
                fd->nxt = NULL;
                (*ls)->tail = fd;
                (*ls)->len--;

            }
            DELSTRING(tmp);
            free (tmp);
        }
        else
        {
            fprintf (stderr, "%s\n", "empty list!");
        }
    }

void popleft (struct list** ls)
{
    if ((*ls) == NULL)
    {
        fprintf (stderr, "%s\n", "Init struct row first !!");
    }
    else
    {
        struct field* tmp;
        struct field* fd = (*ls)->head;

        if (fd)
        {
            tmp = fd;
            if (fd->nxt == NULL)
            {
                (*ls)->len = 0;
                (*ls)->head = NULL;
                (*ls)->tail = NULL;
            }
            else
            {
                tmp = fd;
                fd = fd->nxt;
                fd->prv = NULL;
                (*ls)->head = fd;
                (*ls)->len--;
            }
            DELSTRING(tmp);
            free (tmp);
        }
        else
        {
            fprintf (stderr, "%s\n", "list empty !");
        }
    }
}

bool is_start_head (struct list** ls, uint16_t index)
{
    bool b = (index < (*ls)->len) ? true : false;

    return b;
}

void del_field_by_index (struct list** ls, uint16_t index)
{

    if (index + 1 > (*ls)->len)
    {
        fprintf (stderr, "%s\n", "out of range !");
    }
    else
    {
        if (index == 0)
        {
            popleft (ls);
        }
        else if ((*ls)->len - 1 == index)
        {
            pop (ls);
        }
        else
        {
            struct field* before,* after;
            struct field* fd = (*ls)->head;
            bool test = is_start_head (ls, index);

            if (test)
            {
                while (index > 0)
                {
                    index--;
                    fd = fd->nxt;
                }
            }
            else
            {
                fd = (*ls)->tail;

                index = (*ls)->len - 1 - index;
                while (index > 0)
                {
                    index--;
                    fd = fd->prv;
                }
            }

            before = fd->prv;
            after = fd->nxt;

            before->nxt = after;
            after->prv = before;
            DELSTRING(fd);
            free (fd);
            (*ls)->len--;
        }
    }
}

/* void insert_by_index (struct list** ls, uint16_t index, int value) */
/* { */

/*     if (index + 1 > (*ls)->len) */
/*     { */
/*         fprintf (stderr, "%s\n", "out of range !"); */
/*     } */
/*     else */
/*     { */
/*         if (index == 0) */
/*         { */
/*             appendleft (ls, value); */
/*         } */
/*         else if ((*ls)->len - 1 == index) */
/*         { */
/*             append (ls, value); */
/*         } */
/*         else */
/*         { */
/*             struct field* before,* after; */
/*             struct field* fd = (*ls)->head; */
/*             bool test = is_start_head (ls, index); */

/*             if (test) */
/*             { */
/*                 fd = (*ls)->head; */
/*                 while (index-- > 0) fd = fd->nxt; */
/*             } */
/*             else */
/*             { */
/*                 fd = (*ls)->tail; */
/*                 index = (*ls)->len - 1 - index; */
/*                 while (index-- > 0) fd = fd->prv; */
/*             } */

/*             before = fd->prv; */
/*             after = fd; */

/*             struct field* new = xmalloc (sizeof (*new)); */
/*             new->value = value; */
/*             new->nxt = after; */
/*             new->prv = before; */
/*             before->nxt = new; */
/*             after->prv = new; */
/*             (*ls)->len++; */
/*         } */
/*     } */
/* } */
