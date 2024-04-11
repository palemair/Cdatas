#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define START clock_t end,start = clock()
#define STOP end = clock()
#define TPS printf("\ntemps écoulé : %f \n", (((double)end - start) / CLOCKS_PER_SEC))

void *xmalloc(size_t size)
{
    void* value = malloc (size);

    if (value == NULL)
    {
        perror ("Virtual memory exhausted");
        exit(EXIT_FAILURE);
    }
        
    return value;
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

char *readfile(char* filename)
{
    int fd;
    
    fd = open("t.csv",O_RDONLY,0);
    
    off_t lg = lseek(fd,0,SEEK_END);
    lseek(fd,0,SEEK_SET);
    
    printf("%zu\n",(long)lg);
    char *buffer = xmalloc(lg + 1);

    ssize_t n = read(fd,buffer,lg);

    char *p = buffer;
    *(p + lg + 1) = EOF;

    printf("%zu\n",(long)n);
    
    while(*p != EOF) putchar(*p++);
    
    close(fd);

    return buffer;
}

char* csvtrim(const char * raw)
{
    char *rawfield = strdupa(raw);
    size_t lenraw = strlen(rawfield);
    
    if(lenraw == 0) return "";

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
            if(end-- == begin) return "";
        }
        
        while(isspace(*begin) || *begin == '"') begin++ ;
         
        size_t len = ++end - begin;
        
        char scopy[len + 1];
        memcpy(scopy,begin,len);
        scopy[len] = '\0';

        return strdup(scopy);
    }
}
