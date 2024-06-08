#define _GNU_SOURCE
#include "regexarray.h"
#include "xtools.h"

#define STRLONG "^[-0-9] *+[0-9]* *+$" 
#define STRFLOA "^[-0-9]+[,\\.][0-9]+$" 
#define STRTIME "^[0-9]{2} *: *[0-9]{2}|[0-9]{2} *: *[0-9]{2}$"
#define STRDATE "^[0-9]{2}/[0-9]{2}/[0-9]{2,4}$"
#define STRPERC "^[-0-9]+[,\\.][0-9]+ *%$" 

/* regex pattern array in order to test type of a string */

regexarray* reg_init (void)
{
    int err;
    char* patterns[] = {
        STRLONG,
        STRFLOA,
        STRTIME,
        STRDATE,
        STRPERC
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

void* converter (const char* value, int datatype)
{
    switch (datatype)
    {
    case LONG:
        {
            long* result = xmalloc(sizeof(*result));

            char * endPtr;
            *result = strtol(value, &endPtr, 10 );
            if (value == endPtr)
            {
                fprintf (stderr, "%s\n", "long conversion fail !!");
                return NULL;
            }
            else
            {
                return(result);
            }
            break;
        }
    case FLOAT:
    case PERCENT:
        {
            double *result = xmalloc(sizeof(*result));

            errno = 0;
            *result = strtod (value, NULL);
            if (errno != 0)
            {
                perror ("strtod");
                return NULL;
            }
            else
            {
                return result;
            }
            break;
        }
    case TIME:
    case DATE:
    case NIL:
        {
            return NULL;
            break;
        }
    case STRING:
        {
            char *pt = xmalloc(sizeof(*pt));
            strcpy(pt,value);
            return pt;
            break;
        }
    default:
        {
            return NULL;
            break;
        }
    }
}


/* int main(void) */
/* { */
/*     regexarray* rg = reg_init(); */

/*     char *t = "345.3"; */
    
/*     printf("%s : ",t); */

/*     printf("%d\n",typedata(rg,xtrim(t))); */

/*     double *res; */
    
/*     res = converter(t,typedata(rg,xtrim(t))); */
    
/*     printf("%lf\n",*res); */

/*     free_reg(rg); */

/*     return 0; */
/* } */

