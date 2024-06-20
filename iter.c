#include "xtools.h"
#include "iter.h"

void init_listiter(listiter ils, struct list *ls)
{
   ils->ls = ls;
   ils->curr = NULL; 
   ils->pos = -1;

}

void init_tabiter(tabiter  itb, struct table *tb)
{
   itb->tb = tb;
   itb->curr = NULL; 
   itb->pos = -1;

}
bool next_field(listiter il)
{
    if((il->curr == NULL) && (il->pos == -1))
    {
       il->curr = il->ls->head;
       il->pos++;
       return true;
    }

    else 
    {
        if(il->curr->nxt)
        {
            il->pos++;
            il->curr = il->curr->nxt;
            return true;
        }
        else
        {
            (void)fprintf(stderr,"%s\n","End of list !!");
            return false;
        }
    }
}

bool next_list(tabiter itb)
{
    if((itb->curr == NULL) && (itb->pos == -1))
    {
       itb->curr = itb->tb->t[0];
       itb->pos++;
       return true;
    }

    else 
    {
        if(itb->pos < itb->tb->height -1)
        {
            itb->pos++;
            itb->curr = itb->tb->t[itb->pos];
            return true;
        }
        else
        {
            (void)fprintf(stderr,"%s\n","End of tab !!");
            return false;
        }
    }
}
