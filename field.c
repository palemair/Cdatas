/* handle a csvfile with C */
#define _GNU_SOURCE
#include <string.h>
#include <ctype.h>
#include <wchar.h>
#include <errno.h>
#include "field.h"

/* Assign a value to a field */
int set_field (struct field* fd, char* value, regexarray* rg)
{
    if (rg == NULL)
    {
        fd->nxt = NULL;
        if(value !=NULL)
        { 
            fd->datatype = STRING;
            fd->strdata = strdup (value);
        }
        else
        {
            fd->datatype = NIL;
            fd->strdata = NULL;
        }
        return fd->datatype;
    }
    else
    {
        fd->nxt = NULL;
        fd->datatype = typedata (rg, value);
        switch (fd->datatype)
        {
        case LONG:
            {
                long result = 0;
                char* endPtr;

                result = strtol (value, &endPtr, 10);
                if (value == endPtr)
                {
                    fprintf (stderr, "%s\n", "long conversion fail !!");
                    return ERR;
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
                    return ERR;
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
        case DESC:
            {
                fd->strdata = strdup (value);
                return DESC;
                break;
            }
        case STRING:
            {
                fd->strdata = strdup (value);
                return STRING;
                break;
            }
        default:
            {
                return ERR;
                break;
            }
        }
    }
}

void clear_field (struct field* fd)
{
    if (fd->datatype == STRING) free(fd->strdata);
    fd->datatype = NIL;
    fd->strdata = NULL;
}


void fprint_field(struct field* fd,FILE* outputfile, int fdwidth, int fdprecis)
{
    switch (fd->datatype)
    {
        case LONG:
                {
                    fprintf (outputfile,"%*lld",fdwidth,fd->lgdata);
                    break;
                }

        case FLOAT:
                {
                    fprintf (outputfile,"%*.*f",fdwidth,fdprecis,fd->dbdata);
                    break;
                }
        case NIL:
                {
                    fprintf (outputfile,"%-*.*s",fdwidth,fdwidth,"---");
                    break;
                }
        case STRING:
                {
                    fprintf(outputfile,"%-*.*s",fdwidth,fdwidth,fd->strdata);
                    break;
                }
        case  DESC:
                {
                    char * buff = strdupa(fd->strdata);
                    char result[(strlen (fd->strdata) + 1)];
                    char* res = result;
                    while ((*res++ = toupper (*buff++)));

                    fprintf(outputfile,"%*.*s",fdwidth,fdwidth,result);
                    break;
                }
    }
    printf("%s", "  ");
}
