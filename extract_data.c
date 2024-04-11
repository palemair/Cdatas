/* handle a csvfile with C */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <regex.h>

#define ARRAYSIZE(p) (sizeof(p) / sizeof(p[0]));

regex_t * reg_init(void)
{
   enum { LONG, FLOAT, TIME, DATE };
   int err;

   char *patterns[] = {
                        "^[-0-9][0-9]*+$",
                        "^[-0-9]+[,\\.][0-9]+$",
                        "^[0-9]{2} : [0-9]{2}$",
                        "^[0-9]{2}/[0-9]{2}/[0-9]{2,4}" };

   size_t len = ARRAYSIZE(patterns);
   regex_t *preg = malloc(sizeof(regex_t) * len);

   for(size_t x = 0; x<len; x++)
   {
       err = regcomp (&preg[x], patterns[x], REG_NOSUB | REG_EXTENDED);
       if (err != 0)
       {
         char *text;
         size_t size;

         size = regerror (err, &preg[x], NULL, 0);
         text = malloc (sizeof (*text) * size);
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
   return preg;
}

int main()
{
   const char* type[] = {"entier", "flottant","temps", "date"};
   regex_t* p = NULL;
   p = reg_init();
   const char *str_request[] = { "2", "-1","345","45677887766" };
   int match;

   for(int i=0 ; i<4; i++)
   {
      match = regexec (&p[0], str_request[i], 0, NULL, 0);
      if (match == 0)
         printf ("%s est un %s valide\n", str_request[i], type[0]);
   }

   return 0;
}
