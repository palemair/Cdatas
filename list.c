/* handle a csvfile with C */
#define _GNU_SOURCE
#include <errno.h>
#include "list.h"
#include "xtools.h"

/* Initialize a list */
struct list* init_list (void)
{
    struct list* new = xmalloc (sizeof (*new));

    new->len = 0;
    new->head = NULL;
    new->tail = NULL;

    return new;
}

/* Append a right field and assign a value */
int append (struct list** ls, regexarray* rg, void* value)
{
    if (*ls == NULL)
    {
        fprintf (stderr, "%s\n", "Init struct list first !!");
        return ERR;
    }
    else
    {
        struct field* new = xcalloc (1,sizeof (*new));

        if (set_field (new, value, rg) == ERR)
        {
            fprintf (stderr, "%s\n", "assignement error !!");
            return ERR;
        }
        else
        {
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
    }
    return 1;
}

/* delete the right field */
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
        clear_field (tmp);
        free (tmp);
    }
    else
    {
        fprintf (stderr, "%s\n", "empty list!");
    }
}

/* delete the left field */
void popleft (struct list** ls)
{
    if ((*ls) == NULL)
    {
        fprintf (stderr, "%s\n", "Init struct list first !!");
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
            clear_field (tmp);
            free (tmp);
        }
        else
        {
            fprintf (stderr, "%s\n", "list empty !");
        }
    }
}

/* delete all fields from a list */
void del_list (struct list* ls)
{
    if (ls != NULL)
    {
        struct field* tmp;
        struct field* pfd = ls->head;

        while (pfd)
        {
            tmp = pfd;
            clear_field (tmp);
            pfd = pfd->nxt;
            free (tmp);
        }
        ls->head = NULL;
        ls->tail = NULL;
        ls->len = 0;
    }
}

/* look for the better start of a list */
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
            clear_field (fd);
            free (fd);
            (*ls)->len--;
        }
    }
}
