/* handle a csvfile with C */
#include <ctype.h>
#include "xtools.h"
#include "iter.h"
#include "print.h"

int print_header(struct table *tb)
{
    listiter il = xmalloc(sizeof(*il));
    listiter type = xmalloc(sizeof(*il));
    init_listiter(il, tb->first);
    init_listiter(type, tb->t[1]);
    int lg = 0;
    char* buff = NULL;
    while(next_field(il))
            {
                next_field(type);
                buff = strtoupper(il->curr->strdata);
                switch(type->curr->datatype)
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
                printf("%*s",lg,buff);
            }   
        printf("\n");
        free(il);
        free(type);
        free(buff);
    return 0;
}


int print(struct table* tb, uint32_t start_tab, uint32_t stop_tab)
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
        print_header(tb);
        start_tab = (start_tab == 0)? 1:start_tab;
    }
    listiter il = xmalloc(sizeof(*il));
    for(size_t u = start_tab; u<stop_tab; u++)
    {
        init_listiter(il, tb->t[u]);
        
        while(next_field(il))
        {
            print_field(il->curr, tb->fmtnum,tb->fmtstr,tb->fmtprecis);
        }
        printf("\n");
    }
    free(il);
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
