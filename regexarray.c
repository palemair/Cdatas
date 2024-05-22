#define _GNU_SOURCE
#include "regexarray.h"
#include "csv_type.h"

regexarray* reg_init (void)
{
    int err;
    char* patterns[] = {
        "^[-0-9] *+[0-9]* *+$",      /*long value */
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

/* void* converter (const char* value, int datatype) */
/* { */
/*     int nbconv = 0; */
/*     switch (datatype) */
/*     { */
/*     case LONG: */
/*         { */
/*             long* result = xmalloc(sizeof(*result)); */

/*             nbconv = sscanf (value, "%ld", result); */
/*             if (nbconv != 1) */
/*             { */
/*                 fprintf (stderr, "%s\n", "long conversion fail !!"); */
/*                 return NULL; */
/*             } */
/*             else */
/*             { */
/*                 return(result); */
/*             } */
/*             break; */
/*         } */
/*     case FLOAT: */
/*     case PERCENT: */
/*         { */
/*             double result = 0.0; */

/*             errno = 0; */
/*             result = strtod (value, NULL); */
/*             if (errno != 0) */
/*             { */
/*                 perror ("strtod"); */
/*                 return -1; */
/*             } */
/*             else */
/*             { */
/*                 fd->dbdata = result; */
/*                 return FLOAT; */
/*             } */
/*             break; */
/*         } */
/*     case TIME: */
/*     case DATE: */
/*     case NIL: */
/*         { */
/*             return NULL; */
/*             break; */
/*         } */
/*     case STRING: */
/*         { */
/*             if (NULL == (fd->strdata = strdup (value))) */
/*             { */
/*                 perror ("strdup"); */
/*                 return (EXIT_FAILURE); */
/*             } */
/*             else */
/*             { */
/*                 return STRING; */
/*             } */
/*             break; */
/*         } */
/*     default: */
/*         { */
/*             return NULL; */
/*             break; */
/*         } */
/*     } */
/* } */
