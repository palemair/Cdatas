/* handle a csvfile with C */
#define _GNU_SOURCE
#include <string.h>
#include <time.h>
#include "csv.h"
#include "iter.h"
#include "xtools.h"

/* turing machine parsing */
int parse_csv (char* datas, regexarray * rp, struct table** tb)
{
    typedef enum { ENDL = '\n', DQUOTE = '\"', OUTQ, INQ } position;
    const unsigned char SIZE = 64;
    uint32_t tsize = 2048;
    struct list** tmp = xmalloc (tsize * sizeof (struct list *));
    struct list** curr = tmp;
    *curr = init_list ();
    char* p = datas;
    int c;
    char* cell = xmalloc (SIZE);
    size_t count = 0;
    size_t nb = 1;
    char* pf = NULL;
    position state = OUTQ;
    int DELIM = (*tb)->Dlim;
    
    while ((c = *p++))
    {
        if (count != 0 && ((count % (SIZE - 2)) == 0))
        {
            ++nb;
            cell = xreallocarray (cell, nb * SIZE, sizeof (char));
        }
        if (c == DQUOTE && state == OUTQ)
            state = INQ;

        else if (c == DQUOTE && state == INQ)
            state = OUTQ;

        else if (state == INQ && c == ENDL)
        {
            *(cell + count) = '\\';
            count++;
            *(cell + count) = 'n';
            count++;
            continue;
        }
        else if (state == OUTQ && ((c == DELIM) || (c == ENDL)))
            /* row or field ended */
        {
            *(cell + count) = '\0';
            pf = xtrim (cell);
            int err = append (curr, rp, pf);
            if (!err)
            {
                fprintf (stderr, "%s\n", "append error");
            }
            free (pf);

            if (c == ENDL)
            /* row ended */
            {
                struct list* tmplist = *curr;
                struct field* tmpfd = tmplist->head;
                bool nilrow = true;
                while(tmpfd)
                {
                    nilrow = (tmpfd->datatype != NIL)?false:true;
                    tmpfd=tmpfd->nxt;
                }

                if(nilrow == false)
                {
                    if ((*tb)->height > (tsize - 2))
                    {
                        tsize <<= 1;
                        tmp = xreallocarray (tmp, tsize, sizeof (struct list *));
                        curr = (tmp + (*tb)->height + 1);
                    }
                    else
                    {
                        curr++;
                    }
                *curr = init_list ();
                (*tb)->height++;
                }
                else
                {
                    del_list(tmplist);
                }
            }
            count = 0;
            memset (cell, 0, nb * SIZE);
            continue;
        }
        *(cell + count) = c;
        count++;
    }
    free (*curr);
    free (cell);
    tmp = xreallocarray (tmp, (*tb)->height, sizeof (struct list *));
    (*tb)->t = tmp;
    (*tb)->width = (*tb)->t[0]->len;

    if(((*tb)->header == true) && (*tb)->height>0)
    {
        (*tb)->first = (*tb)->t[0];
    }

    return (EXIT_SUCCESS);
}

struct table* load_csv (char* filename, char delim, bool header)
{
    regexarray* rp = reg_init ();
    char* buffer = xreadfile (filename);
    if (buffer == NULL)
    {
        fprintf (stderr, "%s", "error readfile\n");
        exit(EXIT_FAILURE);
    }

    struct table* tb = init_table (delim, header);
    int n = parse_csv (buffer, rp, &tb);
    if (n != EXIT_SUCCESS)
    {
        fprintf (stderr, "%s", "error parse_csv");
        return NULL;
    }
    free_reg (rp);
    free (buffer);

    return tb;
}


int write_csv (char* filename, struct table *tb)
{
    FILE* outfile = fopen(filename,"w");
    if(outfile == NULL)
    {
        perror("fopen");
        return -1;
    }
    listiter il = xmalloc(sizeof(*il)); 
    int just = 0;
    for(size_t i=0; i<tb->height; i++)
    {
        init_listiter(il, tb->t[i]);
        while(next_field(il)) 
            {
                if(il->curr->datatype == STRING) 
                {
                    just = strlen(il->curr->strdata);
                    fputc('"',outfile);
                    fprint_field(il->curr,outfile,0,just,6);
                    fputc('"',outfile);
                    fprintf(outfile,"%c",tb->Dlim);
                }
                else
                {
                    fprint_field(il->curr,outfile,0,just,6);
                    fprintf(outfile,"%c",tb->Dlim);
                }
            }
            fprintf(outfile,"\n");
    }
    free(il);
    fclose(outfile);

    return 0;
}
