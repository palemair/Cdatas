#define _GNU_SOURCE
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
            /* (void)fprintf(stderr,"%s\n","End of list !!"); */
            return false;
        }
    }
}

bool next_list(tabiter itb)
{
    if((itb->curr == NULL) && (itb->pos == -1))
    {
       if(itb->tb->header)
       {
           itb->curr = itb->tb->t[1];
           itb->pos++;
       }
       else
       {
           itb->curr = itb->tb->t[0];
       }
       itb->pos++;
       return true;
    }

    else 
    {
        if(itb->pos < (int)itb->tb->height -1)
        {
            itb->pos++;
            itb->curr = itb->tb->t[itb->pos];
            return true;
        }
        else
        {
            /* (void)fprintf(stderr,"%s\n","End of tab !!"); */
            return false;
        }
    }
}

int set_field(struct field *fd, char* value, regexarray *rg)
{
    fd->datatype = typedata (rg, value);
    switch (fd->datatype)
    {
    case LONG:
        {
            long result = 0; 

            char * endPtr;
            result = strtol(value, &endPtr, 10 );
            if (value == endPtr)
            {
                fprintf (stderr, "%s\n", "long conversion fail !!");
                return -1;
            }
            else
            {
                fd->lgdata = result;
            }
           return LONG;
           break;
        }
    case FLOAT:
    case PERCENT:
        {
           errno = 0;
           double result;
           result = strtod (value, NULL);

           if (errno != 0)
           {
               perror ("strtod");
               return -1;
           }
           else
           {
               fd->dbdata = result;
           }
           return FLOAT;
           break;
        }
    case TIME:
    case DATE:
    case NIL:
        {
            fd->strdata = NULL;
            return NIL;
            break;
        }
    case STRING:
        {
            fd->strdata = strdup(value);
            return STRING;
            break;
        }
    default:
        {
            return -1;
            break;
        }
    }
}

void clear_field (struct field* fd)
{
    if (fd->datatype == STRING) free (fd->strdata);
    fd->datatype = NIL;
    fd->strdata = NULL;
}

void fprint_field(struct field* fd,FILE* outputfile,int lgnumber,int lgstr, int lgprecision)
{
    switch (fd->datatype)
    {
        case LONG:
                {
                    fprintf (outputfile,"%*ld",lgnumber,fd->lgdata);
                    break;
                }

        case FLOAT:
                {
                    fprintf (outputfile,"%*.*f",lgnumber,lgprecision,fd->dbdata);
                    break;
                }
        case NIL:
                {
                    fprintf (outputfile,"%-*.*s",lgstr,lgstr,"---");
                    break;
                }
        case STRING:
                {
                    fprintf (outputfile,"%-*.*s",lgstr,lgstr,fd->strdata);
                    break;
                }
    }
}

void print_field(struct field* fd,int lgnumber,int lgstr, int lgprecision)
{
    fprint_field(fd,stdout,lgnumber,lgstr,lgprecision);
}
