/* handle a csvfile with C */
#include <ctype.h>
#include "xtools.h"
#include "iter.h"
#include "print.h"

int print_header(struct table *tb)
{
        struct field* fd1 = tb->t[1]->head;
        printf("%6s - ","*");
        while(fd)
        {
            int lg;
            switch(fd1->datatype)
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
            char* buff = strtoupper(fd->strdata);
            printf(" %*s",lg,buff);
            free(buff);
            fd1 = fd1->nxt;
            fd = fd->nxt;
        }
}


int print_table(struct table* tb, uint32_t start_tab, uint32_t stop_tab, size_t start_field, size_t stop_field)
{
    const char *err = "table too short !!";

    if(start_tab > tb->height)
    {
        fprintf(stderr,"%s\n",err);
        return EXIT_FAILURE;
    }
    stop_tab = (stop_tab > tb->height)?tb->height:stop_tab;

    if(tb->header)
    {
        /* print_header(tb); */
    }
    else
    {
        start_tab = (start_tab == 0)? 1:start_tab;
    }

    listiter il = xmalloc(sizeof(*il));
    for(size_t u = start_tab; u<stop_tab; u++)
    {
        init_listiter(il, tb->t[u]);
        for(size_t i=0; i<start_field; i++) next_field(il);
        for(size_t i=0; i<(stop_field -start_field); i++)
        {
            print_field(il->curr, tb->fmtnum,tb->fmtstr);
            next_field(il);
        }
        printf("\n");
    }
    free(il);
return EXIT_SUCCESS;
}

int printall(struct table* tb)
{
    print_table(tb,0,tb->height,0,tb->width);
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
