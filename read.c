/* handle a csvfile with C */
#define _GNU_SOURCE
#include "csv.h"


int main ()
{
    regexarray* rp = reg_init(); 
    char *buffer = readfile("CSV-file/technic.csv");

    if(buffer != NULL) 
    {    
    const unsigned char SIZE = 64;

    char *p = buffer;
    int c;
    char *field = xmalloc(SIZE);
    size_t count = 0;
    size_t nb = 1;
    char *pf = NULL;
    position state = OUTQ;
    /* int delimprv = DELIM; */  

    while((c = *p++))
    {
        if(count != 0 && ((count % (SIZE - 3))== 0))
        {
            ++nb;
            field = xreallocarray(field,nb * SIZE,sizeof(char));
        }
        if(c == '\"' && state == OUTQ) state = INQ;

        else if(c == '\"' && state == INQ) state = OUTQ;

        else if (state == INQ && c == ENDL)
        {
            *(field + count) = '\\';
            count++;
            *(field + count) = 'n';
            count++;
            continue;
        }
        
        else if (state == OUTQ && ((c == DELIM) || (c == ENDL))) 
        {
            *(field + count) = '\0';
            pf = csvtrim(field);
            (c == ENDL)? printf("%s\n",pf) : printf("%s|",pf);
            /* printf("*delimprv = %c*",delimprv); */
            free(pf);
            count = 0;
            memset(field,0,nb*SIZE);
            /* delimprv = c; */
            continue;
        }

        *(field + count) = c;
        count++;
    }

    free(buffer);
    free(field);
    }
    else puts("bad file or error");

    return (EXIT_SUCCESS);
}
