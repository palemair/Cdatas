/* handle a csvfile with C */
#define _GNU_SOURCE
#include <ctype.h>
#include "../include/table.h"
#include "../include/iter.h"
#include "../include/print.h"
#include "../include/xtools.h"

static const char* BEGIN_WD = "| ";
static const char* END_WD = " |";
static const char CROSS = '+';
static const char LINE = '-';
static const char SPACE = ' ';

//print horizontal line
static int lineH (struct table* tb)
{
    if (tb == NULL)
    {


        fprintf (stderr, "%s\n", "Init table first !!");
        return EXIT_FAILURE;
    }
    else
    {
        struct field* fd = tb->desc->head;

        putchar (CROSS);
        for (int i = 0; i < (tb->lgcol1 + 2); i++) putchar (LINE);

        putchar (CROSS);
        while (fd != NULL)
        {
            for (int i = 0; i < (fd->fdlenth + 2); i++) putchar (LINE);
            fd = fd->nxt;
            putchar (CROSS);
        }
        printf ("\n");
    }
    return EXIT_FAILURE;
}

/* print first line containing descriptors */
int print_header (struct table* tb)
{
    if (tb == NULL)
    {
        fprintf (stderr, "%s\n", "Init table first !!");
        return EXIT_FAILURE;
    }

    lineH (tb);
    char* strdesc = "Desc.";

    printf ("%s", BEGIN_WD);
    printf ("%*.*s", tb->lgcol1, (int) strlen (strdesc), strdesc);
    printf ("%s", END_WD);
    struct field* fd = tb->desc->head;

    while (fd)
    {
        putchar (SPACE);
        print_field (fd, fd->fdlenth, fd->fdlenth);
        printf ("%s", END_WD);
        fd = fd->nxt;
    }
    putchar ('\n');
    lineH (tb);
    return EXIT_SUCCESS;
}

/* print simple line nb index */
int printline (struct table* tb, uint32_t index)
{
    if (tb == NULL)
    {
        fprintf (stderr, "%s\n", "Init table first !!");
        return EXIT_FAILURE;
    }
    else
    {
        struct field* fdw = tb->desc->head;
        struct field* fd = tb->t[index]->head;

        printf ("%s", BEGIN_WD);
        printf ("%*u", tb->lgcol1, index);
        printf ("%s", END_WD);
        while (fd)
        {
            putchar (SPACE);
            print_field (fd, fdw->fdlenth, fdw->fdlenth);
            printf ("%s", END_WD);
            fdw = fdw->nxt;
            fd = fd->nxt;
        }
        putchar ('\n');
    }
    return EXIT_SUCCESS;
}

/* print lines from start(included) to stop (not included) */
int printlines (struct table* tb, uint32_t start, uint32_t stop)
{
    if (tb == NULL)
    {
        fprintf (stderr, "%s\n", "Init table first !!");
        return EXIT_FAILURE;
    }
    else
    {
        stop = (stop >= tb->height) ? tb->height : stop;
        for (unsigned u = start; u < stop; u++) printline (tb, u);
        return EXIT_SUCCESS;
    }
}

int printable (struct table* tb)
{
    print_header (tb);
    printlines (tb, 0, tb->height);
    lineH (tb);
    return EXIT_SUCCESS;
}

int r_printable (struct table* tb, unsigned int step)
{
    print_header (tb);
    printlines (tb, 0, step);

    struct field* fdw;
    struct field* fd;
    char* buff = "...";

    for (int i = 0; i < 3; i++)
    {
        fdw = tb->desc->head;
        fd = tb->t[step]->head;
        printf ("%s", BEGIN_WD);
        printf ("%*.*s", tb->lgcol1, tb->lgcol1, buff);
        printf ("%s", END_WD);
        while (fdw)
        {
            putchar (SPACE);
            if (fd->datatype == STRING)
                printf ("%-*.*s", fdw->fdlenth, (int) strlen (buff), buff);
            else
                printf ("%*.*s", fdw->fdlenth, (int) strlen (buff), buff);
            printf ("%s", END_WD);
            fdw = fdw->nxt;
            fd = fd->nxt;
        }
        printf ("\n");
    }
    printlines (tb, (tb->height - step), tb->height);
    lineH (tb);
    return EXIT_SUCCESS;
}

int head (struct table* tb, unsigned int step)
{
    print_header (tb);
    (tb->height > step) ? printlines (tb, 0, step) : printlines (tb, 0, tb->height);

    lineH (tb);

    return EXIT_SUCCESS;
}

int tail (struct table* tb, unsigned int step)
{
    print_header (tb);
    (tb->height > step) ? printlines (tb, (tb->height) - step,
                                      tb->height) : printlines (tb, 0, tb->height);

    lineH (tb);

    return EXIT_SUCCESS;
}
