/* handle a csvfile with C */
#include "print.h"

int print(struct table* tb, uint32_t start, uint32_t stop )
{
    const char *err = "table too short !!";
    if(start > tb->height)
    {
        fprintf(stderr,"%s\n",err);
        return EXIT_FAILURE;
    }
    if(stop > tb->height)
    {
        stop = tb->height;
    }

    struct list** tmp = NULL;
    struct field* f;
    uint8_t lgstr = 15;
    uint8_t lgnumber = 15;
    uint8_t lgprecision = 3;

    if(tb->header)
    {
        struct field* fd = tb->t[0]->head;
        while(fd)
        {
            printf("%*s",lgstr,fd->strdata);
            fd = fd->nxt;
        }
        printf("\n");
        start++;

    }    

    tmp = tb->t + start;
    for (uint32_t u = start; u < stop; u++)
    {
        f = (*tmp++)->head;

        while (f)
        {
            switch (f->datatype)
            {
            case LONG:
                {
                    printf ("%*ld",lgnumber,f->lgdata);
                    break;
                }

            case FLOAT:
                {
                    printf ("%*.*f",lgnumber,lgprecision,f->dbdata);
                    break;
                }
            case NIL:
                {
                    printf ("%*.*s",lgstr,lgstr,"-*-");
                    break;
                }
            case STRING:
                {
                    printf ("%-*.*s",lgstr,lgstr,f->strdata);
                    break;
                }
            }
            f = f->nxt;
        }
        printf ("\n");
    }
return EXIT_SUCCESS;
}

int printall(struct table* tb)
{
    print(tb,0,tb->height);
    return EXIT_SUCCESS;
}

int head(struct table* tb)
{
    (tb->height>10)? print(tb,0,10):print(tb,0,tb->height);
    return EXIT_SUCCESS;
}

int tail(struct table* tb)
{
    size_t start = (tb->height) - 10;
    (tb->height>10)? print(tb,start,tb->height):print(tb,0,tb->height);
    return EXIT_SUCCESS;
}
