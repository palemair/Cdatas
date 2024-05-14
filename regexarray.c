#define _GNU_SOURCE
#include "regexarray.h"

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
