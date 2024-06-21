/* handle a csvfile with C */
#define _GNU_SOURCE
#include <errno.h>
#include "list.h"
#include "iter.h"
#include "xtools.h"

int set_value (struct field* fd, char* value,regexarray* rg)
{
    fd->nxt = NULL;
    int ret = set_field(fd,value,rg);
    if(ret == -1)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

struct list* init_list (void)
{
    struct list* new = xmalloc (sizeof (*new));

    new->len = 0;
    new->head = NULL;
    new->tail = NULL;

    return new;
}

int append (struct list** ls, regexarray* rg, void* value)
{
    if (*ls == NULL)
    {
        fprintf (stderr, "%s\n", "Init struct list first !!");
    }
    else
    {
        struct field* new = xmalloc (sizeof (*new));
        int Error_assign = set_value (new, value, rg);

        if (!Error_assign)
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
    return 1;
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
            clear_field(tmp);
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
            clear_field(tmp);
            free (tmp);
        }
        else
        {
            fprintf (stderr, "%s\n", "list empty !");
        }
    }
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
            clear_field(tmp);
            pfd = pfd->nxt;
            free (tmp);
        }
        ls->head = NULL;
        ls->tail = NULL;
        ls->len = 0;
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
            clear_field(fd);
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
