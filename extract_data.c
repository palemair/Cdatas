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

regexarray * reg_init(void)
{
   int err;

   char *patterns[] = {
                        "^[-0-9][0-9]*+$",                      /*long value*/
                        "^[-0-9]+[,\\.][0-9]+$",                /*double*/
                        "^[0-9]{2} *: *[0-9]{2}|[0-9]{2} *: *[0-9]{2}$",/*time*/
                        "^[0-9]{2}/[0-9]{2}/[0-9]{2,4}$",       /*date*/
                        "^[-0-9]+[,\\.][0-9]+ *%$"               /*percent*/
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

void free_reg(regexarray * rp)
{
   for(int x = 0; x<rp->lgreg_list; x++)
   {
      regfree (&rp->reg_list[x]);
   }
   free(rp->reg_list);
   free(rp);
}

int typedata(regexarray * p,const char * strtest)
{
   if(strlen(strtest) == 0) return(NIL);
   int match;
   regex_t *patterns = p->reg_list;
   int lg =p->lgreg_list;

   for(int type = 0; type<lg; type++)
   {
      match = regexec (&patterns[type],strtest, 0, NULL, 0);
      if(match == 0)
      {
         switch(type)
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

int assign(struct field* fd,const char* value,int datatype)
{

    fd->datatype = datatype;
    int nbconv = 0;
    switch(datatype)
        {
            case LONG :
                {
                    long result = 0;
                    nbconv = sscanf(value,"%ld",&result);
                    if(nbconv != 1) 
                    {
                        fprintf(stderr,"%s\n","long conversion fail !!");
                        return EXIT_FAILURE;
                    }
                    else
                    {
                        fd->lgdata = result;
                    }
                    break;
                }
            case FLOAT : 
            case PERCENT :
                {
                    double result = 0.0;
                    errno = 0;
                    result = strtod(value,NULL);
                    if(errno !=0) 
                    {
                        perror("strtod");
                        return(EXIT_FAILURE);
                    }
                    else
                    {
                        fd->dbdata = result;
                    }
                    break;
                }
            case TIME :
            case DATE :
            case STRING :
                {
                    if(NULL ==(fd->strdata = strdup(value)))
                    {
                       perror("strdup");
                       return(EXIT_FAILURE);
                    }
                    break;
                }
            case NIL :
                {
                    fd->strdata = "-";
                    {
                       perror("strdup");
                       return(EXIT_FAILURE);
                    }
                    break;
                }
        }
    return EXIT_SUCCESS; 
}



/* TEST ZONE */
/* --------- */

/* int main() */
/* { */
/*     const char *test = "ERFFF   EF34564"; */
   
/*     regexarray* rp = reg_init(); */

/*     struct field fd ; */
/*     struct field* pt = &fd ; */

/*     int n = 0; */ 
/*         n = typedata(rp,test); */
/*         assign(pt,test,n); */

/*         switch(pt->datatype) */
/*         { */
/*             case LONG: */ 
/*                 { */
/*                     printf("%s : %d\n",test,pt->datatype); */
/*                     printf("%s : %ld\n",test,pt->lgdata); */
/*                     break; */
/*                 } */

/*             case FLOAT: */ 
/*                 { */
/*                     printf("%s : %d\n",test,pt->datatype); */
/*                     printf("%s : %f\n",test,pt->dbdata); */
/*                     break; */
/*                 } */
/*             case STRING: */ 
/*                 { */
/*                     printf("%s : %d\n",test,pt->datatype); */
/*                     printf("%s" ,pt->strdata); */
/*                     break; */
/*                 } */
/*         } */

/*     free_reg(rp); */

/*     return 0; */
/* } */
