/* handle a csvfile with C */
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>

void * xmalloc(size_t size)
{
    void *p = malloc(size);
    if(size == 0) size=1;
    
    if(p == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    return p;
}

void *xreallocarray(void *ptr, size_t nmemb,size_t size)
{
    void*value = reallocarray (ptr, nmemb, size);
    if (value == 0)
    {
        perror ("Virtual memory exhausted");
        exit(EXIT_FAILURE);
    }
    return value;
}

char* csvtrim(const char * raw)
{
    char *rawfield = strdupa(raw);
    size_t count = strlen(rawfield);
    
    if(count == 0) return "";

    else
    {
        char *begin = rawfield;
        char *end = rawmemchr(begin,'\0');
        if(end == NULL) 
        {
            perror("rawmemchr");
            return NULL;
        }

        while(isspace(*end) || *end == '\0' || *end == '"') 
        {
            count--;
            if((count) == 0) break;
            end-- ;
        }
        if(end == begin) return "";
        
        while(isspace(*begin) || *begin == '"') begin++ ;
         
        size_t len = ++end - begin;
        
        char scopy[len + 1];
        memcpy(scopy,begin,len);
        scopy[len] = '\0';

        return strdup(scopy);
    }

}

int main ()
{

#define SIZE 64

    FILE * fi = fopen("CSV-file/test.csv","r");
    if(fi == NULL) 
    {
        perror("fopen");
        return EXIT_FAILURE;
    }
    
    int c;  
    char *field = xmalloc(SIZE);
    size_t count = 0;
    size_t nb = 1;
    char *pf = NULL;
    typedef enum position { OUTQ, INQ , DELIM = '|' , ENDL = '\n'} position;
    position etat = OUTQ;
    int delimprv = ENDL;  

    while((c = getc(fi)) != EOF)
    {
        if(count != 0 && ((count % (SIZE - 3))== 0))
        {
            ++nb;
            field = xreallocarray(field,nb * SIZE,sizeof(char));
        }
        if(c == '\"' && etat == OUTQ) etat = INQ;

        else if(c == '\"' && etat == INQ) etat = OUTQ;

        else if (etat == INQ && c == ENDL)
        {
            *(field + count) = '\\';
            count++;
            *(field + count) = 'n';
            count++;
            continue;
        }
        
        else if (etat == OUTQ && ((c == DELIM) || (c == ENDL))) 
        {
            *(field + count) = '\0';
            pf = csvtrim(field);
            (c == ENDL)? printf("%s\n",pf) : printf("%s",pf);
            printf("*delimprv = %c*",delimprv);
            free(pf);
            count = 0;
            memset(field,0,nb*SIZE);
            delimprv = c;
            continue;
        }

        *(field + count) = c;
        count++;
    }

    fclose(fi);
    free(field);

    return (EXIT_SUCCESS);
}
