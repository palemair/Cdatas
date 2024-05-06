/*
 * =====================================================================================
 *
 *       Filename:  extract_data.c
 *
 *    Description:  test value from csv with regex 
 *
 *        Version:  1.0
 *        Created:  15/04/2024 09:06:27
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  LP 
 *   Organization:  
 *
 * =====================================================================================
 */

#define _GNU_SOURCE
#include "csv.h"

regexarray* reg_init (void)
{
    int err;
    char* patterns[] = {
        "^[-0-9][0-9]*+$",      /*long value */
        "^[-0-9]+[,\\.][0-9]+$",        /*double */
        "^[0-9]{2} *: *[0-9]{2}|[0-9]{2} *: *[0-9]{2}$",        /*time */
        "^[0-9]{2}/[0-9]{2}/[0-9]{2,4}$",       /*date */
        "^[-0-9]+[,\\.][0-9]+ *%$"      /*percent */
    };
    regexarray* regarray = malloc (sizeof (*regarray));
    regarray->lgreg_list = ARRAYSIZE (patterns);
    regex_t* preg = malloc (sizeof (regex_t) * regarray->lgreg_list);

    for (size_t x = 0; x < regarray->lgreg_list; x++)
    {
        err = regcomp (&preg[x], patterns[x], REG_NOSUB | REG_EXTENDED);
        if (err != 0)
        {
            size_t size = regerror (err, &preg[x], NULL, 0);
            char text[size + 1];
            regerror (err, &preg[x], text, size);
            fprintf (stderr, "%s\n", text);
        }
    }
    regarray->reg_list = preg;
    return regarray;
}

void free_reg (regexarray * rp)
{
    for (int x = 0; x < rp->lgreg_list; x++)
    {
        regfree (&rp->reg_list[x]);
    }
    free (rp->reg_list);
    free (rp);
}

int typedata (regexarray * p, const char* strtest)
{
    if (strtest == NULL)
    {
        return (NIL);
    }
    else
    {
        int match;
        regex_t* patterns = p->reg_list;
        int lg = p->lgreg_list;

        for (int type = 0; type < lg; type++)
        {
            match = regexec (&patterns[type], strtest, 0, NULL, 0);
            if (match == 0)
            {
                switch (type)
                {
                case 0:
                    return (LONG);
                case 1:
                    return (FLOAT);
                case 2:
                    return (TIME);
                case 3:
                    return (DATE);
                case 4:
                    return (PERCENT);
                }
            }
        }
        return (STRING);
    }
}

int assign (struct field* fd, const char* value, int datatype)
{
    fd->datatype = datatype;
    int nbconv = 0;
    switch (fd->datatype)
    {
    case LONG:
        {
            long result = 0;

            nbconv = sscanf (value, "%ld", &result);
            if (nbconv != 1)
            {
                fprintf (stderr, "%s\n", "long conversion fail !!");
                return -1;
            }
            else
            {
                fd->lgdata = result;
                return LONG;
            }
            break;
        }
    case FLOAT:
    case PERCENT:
        {
            double result = 0.0;

            errno = 0;
            result = strtod (value, NULL);
            if (errno != 0)
            {
                perror ("strtod");
                return -1;
            }
            else
            {
                fd->dbdata = result;
                return FLOAT;
            }
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
            if (NULL == (fd->strdata = strdup (value)))
            {
                perror ("strdup");
                return (EXIT_FAILURE);
            }
            else
            {
                return STRING;
            }
            break;
        }
    default:
        {
            return -1;
            break;
        }
    }
}
int try_assign(struct field* f,void* value,regexarray* rg)
{
        f->nxt = NULL;
        f->datatype = typedata (rg, value);

        return assign (f, value, f->datatype);
}

/* int main() */
/* { */
/*     regexarray* rp = reg_init (); */
    
/*     int n = typedata(rp,"3455445"); */
/*     printf("%d\n",n); */

/*     free_reg (rp); */

/* } */
