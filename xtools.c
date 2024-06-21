/* #define _GNU_SOURCE */
#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "xtools.h"

/* malloc custom to avoid error test */
void* xmalloc (size_t size)
{
    void* value = NULL;

    value = malloc (size);

    if (value == NULL)
    {
        perror ("Virtual memory exhausted");
        exit (EXIT_FAILURE);
    }

    return value;
}

/* realloc custom to avoid error test */
void* xreallocarray (void* ptr, size_t nmemb, size_t size)
{
    void* value = realloc (ptr, nmemb * size);

    if (value == 0)
    {
        perror ("Virtual memory exhausted");
        exit (EXIT_FAILURE);
    }
    return value;
}

/* delete left and right space char + double quotes in a char string */
char* xtrim (const char* raw)
{
    char* rawfield = strdupa(raw);
    char* begin = rawfield;
    char* end = rawmemchr (begin, '\0');

    if (end == NULL)
    {
        perror ("rawmemchr failed, not à correct string !!");
        return NULL;
    }

    while (isspace (*end) || *end == '\0' || *end == '"')
    {
        if (end-- == begin)
        {
            return NULL;
        }
    }

    while (isspace (*begin) || *begin == '"')
        begin++;

    size_t len = ++end - begin;

    char *scopy = xmalloc(sizeof(char) * (len + 1));

    memcpy (scopy, begin, len);
    scopy[len] = '\0';

    return scopy;
}

/* load an entire file in a char buffer */
char* xreadfile (char* filename)
{
    int fd = 0;
    char* buffer = NULL;

    struct stat* bufstat = xmalloc(sizeof(*bufstat));
    stat(filename,bufstat);
    
    off_t lg = bufstat->st_size;
    free(bufstat);

    if ((fd = open (filename, O_RDONLY, 0)) != -1)
    {
        if (lg > 0)
        {
            buffer = xmalloc (lg + 1);
            ssize_t n = read (fd, buffer, lg);
            if (n == -1)
            {
                perror ("read error");
                close (fd);
                return NULL;
            }
            else
            {
                *(buffer + lg) = '\0';
                close (fd);
                return buffer;
            }
        }
        else
        {
            close (fd);
            return NULL;
        }
    }
    else
    {
        perror ("readfile");
        return NULL;
    }
}

char* strtoupper(const char* src)
{
    char* copy = strdupa(src);
    char* pt = copy;
    char* res = xmalloc(strlen(src) + 1);
    char* result = res;
    
    while((*res++ = toupper(*pt++)));
    return(result);
}
