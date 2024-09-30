/* handle a csvfile with C */
#define _GNU_SOURCE
#include <stdarg.h>
#include <ctype.h>
#include "xtools.h"
#include "table.h"
#include "print.h"

//print the descriptors
int print_header (struct table* tb)
{
    int lg = 0;
    char* buff = NULL;
    struct field* fd0,* fd1;

    fd1 = tb->t[1]->head;
    fd0 = tb->t[0]->head;
    printf (" %s | ", "Desc");
    if (tb->header)
    {

        while (fd0)
        {
            buff = strdupa (fd0->strdata);
            char* pt = buff;
            char* res = xmalloc (strlen (fd0->strdata) + 1);
            char* result = res;

            while ((*res++ = toupper (*pt++)));

            switch (fd1->datatype)
            {
            case STRING:
                {
                    lg = tb->fmtstr;
                    break;
                }

            default:
                {
                    lg = tb->fmtnum;
                    break;
                }
            }
            printf ("%*s", lg, result);
            fd0 = fd0->nxt;
            fd1 = fd1->nxt;
        }
        printf ("\n");
    }
    else
    {
        char vindex[20] = "   A";
        char c = 'A';

        for (int i = 0; i < tb->width; i++)
        {
            vindex[3] = c;

            switch (fd1->datatype)
            {
            case STRING:
                {
                    lg = tb->fmtstr;
                    break;
                }

            default:
                {
                    lg = tb->fmtnum;
                    break;
                }
            }
            printf ("%*s", lg, vindex);
            fd1 = fd1->nxt;
            c++;
        }
        printf ("\n");
    }
    return 0;
}

void printlines (struct table* tb, uint32_t start, uint32_t stop)
{
    struct field* fd;

    if ((tb->header) && (start == 0)) start = 1;

    stop = (stop >= tb->height) ? tb->height : stop;

    for (unsigned u = start; u < stop; u++)
    {
        fd = tb->t[u]->head;
        printf ("%5u | ", u);

        while (fd)
        {
            fprint_field (fd, stdout, tb->fmtstr, tb->fmtnum, tb->fmtprecis);
            fd = fd->nxt;
        }
        putchar ('\n');
    }
}

void printable (struct table* tb)
{
    print_header (tb);
    printlines (tb, 0, tb->height);
}

void r_printable (struct table* tb, unsigned int step)
{
    print_header (tb);
    printlines (tb, 0, step);

    struct field* fd0,* fd1;
    int lg;
    char *buff = "---";

    for (int i = 0; i < 2; i++)
    {
        fd1 = tb->t[1]->head;
        fd0 = tb->t[0]->head;
        printf ("  %s | ", buff);
        while (fd0)
        {
            switch (fd1->datatype)
            {
            case STRING:
                {
                    lg = tb->fmtstr;
                    printf ("%-*s", lg, buff);
                    break;
                }

            default:
                {
                    lg = tb->fmtnum;
                    printf ("%*s", lg, buff);
                    break;
                }
            }
            fd0 = fd0->nxt;
            fd1 = fd1->nxt;
        }
        printf ("\n");
    }
    printlines (tb, (tb->height - step), tb->height);
}

int head(struct table* tb)
{
    print_header (tb);
    (tb->height>10)? printlines(tb,0,10):printlines(tb,0,tb->height);
    return EXIT_SUCCESS;
}

int tail(struct table* tb)
{
    print_header (tb);
    (tb->height>10)? printlines(tb,(tb->height)-10,tb->height):printlines(tb,0,tb->height);
    return EXIT_SUCCESS;
}
