/* handle a csvfile with C */
#include <ctype.h>
#include "xtools.h"
#include "iter.h"
#include "print.h"

enum { lgstr = 25, lgnumber = 10, lgprecision = 2};

int print(struct table* tb, uint32_t start, uint32_t stop )
{
    const char *err = "table too short !!";

    if(start > tb->height)
    {
        fprintf(stderr,"%s\n",err);
        return EXIT_FAILURE;
    }
    stop = (stop > tb->height)?tb->height:stop;

    struct list** tmp = NULL;
    struct field* f;

    if(tb->header)
    {
        struct field* fd = tb->t[0]->head;
        struct field* fd1 = tb->t[1]->head;
        printf("%6s - ","*");
        while(fd)
        {
            int lg;
            switch(fd1->datatype)
            {
                case STRING:
                    {
                        lg = lgstr;
                        break;
                    }
                    
                default:
                    {
                        lg = lgnumber;
                        break;
                    }
            }
            char* buff = strtoupper(fd->strdata);
            printf(" %*s",lg,buff);
            free(buff);
            fd1 = fd1->nxt;
            fd = fd->nxt;
        }
        printf("\n");
        if(start == 0) start++;
    }    

    tmp = tb->t + start;
    for (uint32_t u = start; u < stop; u++)
    {
        f = (*tmp++)->head;

        printf("%06u - ",u);
        while (f)
        {
            /* printf("Type %d : ", f->datatype); */
            switch (f->datatype)
            {
            case LONG:
                {
                    printf (" %*ld",lgnumber,f->lgdata);
                    break;
                }

            case FLOAT:
                {
                    printf (" %*.*f",lgnumber,lgprecision,f->dbdata);
                    break;
                }
            case NIL:
                {
                    printf (" %*.*s",lgstr,lgstr,"-*-");
                    break;
                }
            case STRING:
                {
                    printf (" %-*.*s",lgstr,lgstr,f->strdata);
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
    (tb->height>10)? print(tb,(tb->height)-10,tb->height):print(tb,0,tb->height);
    return EXIT_SUCCESS;
}

