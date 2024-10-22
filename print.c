/* handle a csvfile with C */
#define _GNU_SOURCE
#include <ctype.h>
#include "table.h"
#include "iter.h"
#include "print.h"
#include "xtools.h"

static int lg = 5;

//print horizontal line
uint16_t* mkwidth(struct table* tb)
{
    uint16_t* tmp = xmalloc(sizeof(*tmp) * tb->width); 
    uint16_t* p = tmp;
    struct field* tf = tb->desc->head;
    
    while(tf != NULL)
    {
      *p++=tf->just;
      tf = tf->nxt;
    }
    
    iterator it = init_iter(tb);
    while(next_iter(it)) 
            {
                tmp[it->xpos] = (tmp[it->xpos] >= it->curr->just) ? tmp[it->xpos] : it->curr->just;
            }
    destroy_iter(it);

    return tmp;
}

void lineH(struct table* tb, int lg,uint16_t* wd)
{
    printf("%c",'+');
    for(int i =0; i<(lg + 2); i++) printf("%c",'-');
    
    printf("%c",'+');
    for(int u =0; u<(tb->width) ; u++)
    {
        for(int i =0; i<(wd[u] + 2) ; i++) printf("%c",'-');
        printf("%c",'+');
    }
    printf ("\n");
}
    
int print_header (struct table* tb)
{
    uint16_t* pt = mkwidth(tb);
    
    lineH(tb,lg,pt);

    printf ("| %*s |",lg,"Desc");
    if (tb->header)
    {
        struct field* fd = tb->desc->head;
        int u = 0;
        while (fd)
        {
            print_field(fd, pt[u], pt[u]);
            u++;
            fd = fd->nxt;
        }
        putchar ('\n');
    }
    else
    {
        char *pts = NULL;
        
        for(unsigned int i = 1; i<=tb->width;i++)
        {
            pts = baseA(i);
            printf(" %*s |",tb->fdwidth,pts);
         }
        putchar('\n');
    }

    lineH(tb,lg,pt);
    free(pt);
    return 0;
}

void printline (struct table* tb, uint32_t index)
{
    uint16_t* pt = mkwidth(tb);

    if (tb == NULL)
    {
        fprintf (stderr, "%s\n", "Init table first !!");
    }
    else
    {
        struct field* fd = tb->t[index]->head;
        int u=0;
        while (fd)
        {
            print_field(fd, pt[u], pt[u]);
            u++;
            fd = fd->nxt;
        }
        putchar ('\n');
    }
    free(pt);
}

void printlines (struct table* tb, uint32_t start, uint32_t stop)
{
    uint16_t* pt = mkwidth(tb);
    if (tb == NULL)
    {
        fprintf (stderr, "%s\n", "Init table first !!");
    }
    else
    {
        struct field* fd;
        stop = (stop >= tb->height) ? tb->height : stop;

        for (unsigned u = start; u < stop; u++)
        {
            fd = tb->t[u]->head;
            printf ("| %*u |",lg, u);
            int v=0;
            while (fd)
            {
                print_field (fd, pt[v], pt[v]);
                v++;
                fd = fd->nxt;
            }
            putchar ('\n');
        }
    }
    free(pt);
}

void printable (struct table* tb)
{
    print_header (tb);
    printlines (tb, 0, tb->height);
    uint16_t* pt = mkwidth(tb);
    lineH(tb,lg,pt);
    free(pt);
}

void r_printable (struct table* tb, unsigned int step)
{
    print_header (tb);
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
    uint16_t* pt = mkwidth(tb);
    lineH(tb,lg,pt);
    free(pt);
}

int head(struct table* tb,unsigned int step)
{
    print_header (tb);
    (tb->height>step)? printlines(tb,0,step):printlines(tb,0,tb->height);
    uint16_t* pt = mkwidth(tb);
    lineH(tb,lg,pt);
    free(pt);

    return EXIT_SUCCESS;
}

int tail(struct table* tb, unsigned int step)
{
    print_header (tb);
    (tb->height>step)? printlines(tb,(tb->height)-step,tb->height):printlines(tb,0,tb->height);
    uint16_t* pt = mkwidth(tb);
    lineH(tb,lg,pt);
    free(pt);

    return EXIT_SUCCESS;
}
