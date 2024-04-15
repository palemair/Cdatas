/* handle a csvfile with C */
#define _GNU_SOURCE
#include "csv.h"

int parse_csv(char *datas, regexarray* rp, struct table** tb)
{
    const unsigned char SIZE = 64;
    struct list **curr = (*tb)->t;
    *curr = init_list();
    char *p = datas;
    int c;
    char *cell = xmalloc(SIZE);
    size_t count = 0;
    size_t nb = 1;
    char *pf = NULL;
    position state = OUTQ;

    while((c = *p++))
    {
        if(count != 0 && ((count % (SIZE - 3))== 0))
        {
            ++nb;
            cell = xreallocarray(cell,nb * SIZE,sizeof(char));
        }
        if(c == DQUOTE && state == OUTQ) state = INQ;

        else if(c == DQUOTE && state == INQ) state = OUTQ;

        else if (state == INQ && c == ENDL)
        {
            *(cell + count) = '\\';
            count++;
            *(cell + count) = 'n';
            count++;
            continue;
        }
        
        else if (state == OUTQ && ((c == DELIM) || (c == ENDL))) 
        {
            *(cell + count) = '\0';
            pf = csvtrim(cell);
            
            int err = append(curr, rp, pf);        
            if(err) fprintf(stderr,"%s\n","append error");

            if(c == ENDL)
            {
                curr++;
                *curr = init_list();
                (*tb)->height++;
            }
            free(pf);
            count = 0;
            memset(cell,0,nb*SIZE);
            continue;
        }

        *(cell + count) = c;
        count++;
    }

    free(cell);

    return (EXIT_SUCCESS);
}


int main()
{
    char* filename = "CSV-file/fichier.csv";
    char delim = ',';
    bool header = false;

    regexarray* rp = reg_init(); 
    char *buffer = readfile(filename);

    struct table* tb;
    tb = init_table(delim,header, 1024);
    START;
    parse_csv(buffer,rp,&tb);
    STOP;

    TPS("parse_csv");

    struct list* tmp = NULL;
    struct field* f; 

    for(int u=0; u< (int)tb->height;u++)
    {
        tmp = tb->t[u];
        f = tmp->head;
        tb->header == true ? printf("%d > ",u) : printf("%d > ",u+1);
        while(f)
        {
        switch(f->datatype)
        {
            case LONG: 
                {
                    printf("%10ld ",f->lgdata);
                    break;
                }

            case FLOAT: 
                {
                    printf("%10.2f ",f->dbdata);
                    break;
                }
            case NIL: 
            case STRING: 
                {
                    printf("%10.10s " ,f->strdata);
                    break;
                }
        }
            f = f->nxt;
        }
        printf("\n");
    }

    free_reg(rp);
    free(buffer);

    return EXIT_SUCCESS;
}
