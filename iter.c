#define _GNU_SOURCE
#include "iter.h"
#include "xtools.h"

iterator init_iter(struct table *tb)
{
   iterator it = xcalloc(1,sizeof(*it));

   it->tb = tb;
   it->curr = NULL; 
   it->xpos = -1;
   it->ypos = -1;
   return it;
}

void destroy_iter(iterator it)
{
    free(it);
}

uint8_t next_iter(iterator it)
{
    if((it->curr == NULL) && (it->ypos == -1) && (it->xpos == -1))
    {
       it->curr = it->tb->t[0]->head;
       it->ypos ++;
       it->xpos ++;
       return 2;
    }

    else 
    {
       if(it->curr->nxt != NULL)
       {
            it->curr = it->curr->nxt;
                it->xpos++;
                return 1;
            }
            else 
            {
                it->ypos++;
                if((unsigned long)it->ypos>=it->tb->height) 
                {
                    return 0;
                }
                else
                {
                    it->curr = it->tb->t[it->ypos]->head;
                    it->xpos = 0;
                    return 2;
                }
            }
    }
    return 0;
}

void fprint_iter(iterator it,FILE* outputfile)
{
    fprint_field(it->curr,outputfile,it->tb->fdwidth,it->tb->fdprecis);
}

void print_iter(iterator it)
{
    struct field* fd = it->curr;
    print_field(fd,fd->just,fd->just);
}
