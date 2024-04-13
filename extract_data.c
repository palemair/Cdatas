/* Test value */
#define _GNU_SOURCE
#include "csv.h"

regexarray * reg_init(void)
{
   int err;

   char *patterns[] = { "^[-0-9][0-9]*+$",                      /*long value*/
                        "^[-0-9]+[,\\.][0-9]+$",                /*double*/
                        "^[0-9]{2}:[0-9]{2}|[0-9]{2}:[0-9]{2}$",/*time*/
                        "^[0-9]{2}/[0-9]{2}/[0-9]{2,4}$",       /*date*/
                        "^[-0-9]+[,\\.][0-9]+ %$"               /*percent*/
                        };                                  /*END*/
      
   regexarray *regarray = malloc(sizeof(* regarray));
   regarray->lgreg_list = ARRAYSIZE(patterns);

   regex_t *preg = malloc(sizeof(regex_t) * regarray->lgreg_list);

   for(size_t x = 0; x<regarray->lgreg_list; x++)
   {
       err = regcomp (&preg[x], patterns[x], REG_NOSUB | REG_EXTENDED);
       if (err != 0)
       {
         size_t size = regerror (err, &preg[x], NULL, 0);
         char *text = malloc (sizeof (*text) * size);
         if (text)
         {
            regerror (err, &preg[x], text, size);
            fprintf (stderr, "%s\n", text);
            free (text);
         }
         else
         {
            fprintf (stderr, "Memoire insuffisante\n");
            exit (EXIT_FAILURE);
         }
        }
   }
   regarray->reg_list = preg;
   return regarray;
}

int typedata(regexarray * p,const char * strtest)
{
   int match;
   regex_t *patterns = p->reg_list;
   int lg =p->lgreg_list;

   for(int x = 0; x<lg; x++)
   {
      match = regexec (&patterns[x],strtest, 0, NULL, 0);
      if(match == 0)
      {
         switch(x)
         {
             case LONG :    return(LONG);
             case FLOAT :   return(FLOAT);
             case TIME :    return(TIME); 
             case DATE :    return(DATE); 
             case PERCENT : return(PERCENT);
         }
      }
   }
   return(STRING);
}

void free_reg(regexarray * rp)
{
   for(int x = 0; x<rp->lgreg_list; x++)
   {
      regfree (&rp->reg_list[x]);
   }
   free(rp->reg_list);
   free(rp);
}

/* TEST ZONE */
/* --------- */

/* int main() */
/* { */

/*     const char *test[] = {"-34566774", "-34556.45", "Elkdjd345,45   ", "23/12/19", "200,34 %", "12:23:34"}; */
   
/*     regexarray* rp = reg_init(); */
    
/*     int n = 0; */ 
/*     int lg = ARRAYSIZE(test); */
/*     for(int u = 0; u<lg; u++) */
/*     { */
/*         n = typedata(rp,test[u]); */
/*         printf("%s : %d\n",test[u],n); */
/*     } */

/*     free_reg(rp); */

/*     return 0; */
/* } */
