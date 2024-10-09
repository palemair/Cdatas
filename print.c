/* handle a csvfile with C */
#define _GNU_SOURCE
#include <ctype.h>
#include "xtools.h"
#include "table.h"
#include "print.h"

static int lg = 6;

//print the descriptors
int print_header (struct table* tb)
{
    printf ("%*s",lg+3,"Desc | ");

    if (tb->header)
    {
        printline(tb,0);
    }
    else
    {
        char vindex[20] = "   A";
        char c = 'A';

        for (int i = 0; i < tb->width; i++)
        {
            vindex[3] = c;
            printf ("%*s", tb->fdwidth, vindex);
            c++;
        }
        printf ("\n");
    }
    for(int i =0; i<7; i++)
    {
        printf("%c",'-');
    }
    printf("%c",'+');
    for(int i =0; i<((tb->fdwidth) * (tb->width)) + (lg-1) ; i++)
    {
        printf("%c",'-');
    }
    printf ("\n");
    return 0;
}

void printline (struct table* tb, uint32_t index)
{
    if (tb == NULL)
    {
        fprintf (stderr, "%s\n", "Init table first !!");
    }
    else
    {
        struct field* fd = tb->t[index]->head;

        while (fd)
        {
            fprint_field (fd, stdout, tb->fdwidth, tb->fdprecis);
            fd = fd->nxt;
        }
        putchar ('\n');
    }
}

void printlines (struct table* tb, uint32_t start, uint32_t stop)
{
    struct field* fd;

    if(tb->header) start = (start == 0) ? 1:start;
    stop = (stop >= tb->height) ? tb->height : stop;

    for (unsigned u = start; u < stop; u++)
    {
        fd = tb->t[u]->head;
        printf ("%*u | ",lg, u);

        while (fd)
        {
            fprint_field (fd, stdout, tb->fdwidth, tb->fdprecis);
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
    /* print_header (tb); */
    printlines (tb, 0, step);

    struct field* fd = tb->t[step]->head;
    char *buff = "... |";

    for (int i = 0; i < 2; i++)
    {
        printf ("%*.*s",lg,lg-2,buff);
        while(fd)
        {
            if(fd->datatype == STRING || fd->datatype == NIL)
                printf("%-*s",tb->fdwidth, buff);
            else
                printf("%*s",tb->fdwidth, buff);

        }
        printf ("\n");
    }
    printlines (tb, (tb->height - step), tb->height);
}

int head(struct table* tb,unsigned int step)
{
    print_header (tb);
    (tb->height>step)? printlines(tb,0,step):printlines(tb,0,tb->height);
    return EXIT_SUCCESS;
}

int tail(struct table* tb, unsigned int step)
{
    print_header (tb);
    (tb->height>step)? printlines(tb,(tb->height)-step,tb->height):printlines(tb,0,tb->height);
    return EXIT_SUCCESS;
}
