/* handle a csvfile with C */
#define _GNU_SOURCE
#include "csv.h"

int parse_csv (char *datas, regexarray * rp, struct table **tb)
{
   const unsigned int k = 2;
   const unsigned char SIZE = 64;
   uint32_t tsize = 1024;
   struct list **tmp = xmalloc(tsize * sizeof(struct list*));
   struct list **curr = tmp;

   *curr = init_list ();
   char *p = datas;
   int c;
   char *cell = xmalloc (SIZE);
   size_t count = 0;
   size_t nb = 1;
   char *pf = NULL;
   position state = OUTQ;

   uint16_t curr_width, max_width;
   curr_width = 0;
   max_width = 0;

   while ((c = *p++))
   {
      if (count != 0 && ((count % (SIZE - 3)) == 0))
      {
         ++nb;
         cell = xreallocarray (cell, nb * SIZE, sizeof (char));
      }
      if (c == DQUOTE && state == OUTQ)
         state = INQ;

      else if (c == DQUOTE && state == INQ)
         state = OUTQ;

      else if (state == INQ && c == ENDL)
      {
         *(cell + count) = '\\';
         count++;
         *(cell + count) = 'n';
         count++;
         continue;
      }

      else if (state == OUTQ && ((c == DELIM) || (c == ENDL)))
      {
         *(cell + count) = '\0';
         curr_width++;

         pf = csvtrim (cell);

         int err = append (curr, rp, pf);
         if (err)
            fprintf (stderr, "%s\n", "append error");

         if (c == ENDL)
         {
            max_width = (curr_width >= max_width) ? curr_width : max_width;

            if((*tb)->height > (tsize - 2))
            {
                /* k++; */
                tmp = xreallocarray(tmp, k * tsize,sizeof(struct list*));
                tsize <<= 1;
                curr = (tmp + (*tb)->height + 1);
                *curr = init_list ();
            }
            else
            {
                curr++;
                *curr = init_list ();
            }
            (*tb)->height++;
            curr_width = 0;
         }
         free (pf);
         count = 0;
         memset (cell, 0, nb * SIZE);
         continue;
      }

      *(cell + count) = c;
      count++;
   }

   free (cell);
   tmp = xreallocarray(tmp, (*tb)->height,sizeof(struct list*));
   (*tb)->t = tmp;
   (*tb)->width = max_width;
   return (EXIT_SUCCESS);
}

struct table* load_csv(char* filename, char delim, bool header)
{
   regexarray *rp = reg_init ();
   char *buffer = readfile (filename);
   if(buffer == NULL)
   {
       fprintf(stderr,"%s","error readfile");
       return NULL;
   }

   struct table *tb = init_table (delim, header);
   START;
   parse_csv (buffer, rp, &tb);
   STOP;
   TPS ("parse_csv");

   free_reg (rp);
   free (buffer);

   return tb;
}

int main ()
{
   struct table *tb = load_csv("xaa",',',false);


   struct list *tmp = NULL;
   struct field *f;
    
   for (int u = 0; u < (int) tb->height; u++)
   {
      tmp = tb->t[u];
      f = tmp->head;
      if(u==10)
      {
           puts("       .............");
           printf("\n");
      }
      if((u>10) && (u< (int) tb->height -10)) 
          continue;

      tb->header == true ? printf ("%5d > ", u) : printf ("%5d > ", u + 1);
      while (f)
      {
         switch (f->datatype)
         {
         case LONG:
            {
               printf ("%20ld ", f->lgdata);
               break;
            }

         case FLOAT:
            {
               printf ("%20.2f ", f->dbdata);
               break;
            }
         case NIL:
            {
               printf ("%20.20s ", f->strdata);
               break;
            }
         case STRING:
            {
               printf ("%-20.20s ", f->strdata);
               break;
            }
         }
         f = f->nxt;
      }
      printf ("\n");
   }

/* freetable */
   /* for (int u = 0; u < (int) tb->height; u++) */
   /* { */
   /*    tmp = tb->t[u]; */
   /*    f = tmp->tail; */
   /*    while (f) */
   /*    { */
   /*       f = f->prv; */
   /*    } */
   /*    /1* printf ("\n"); *1/ */
   /* } */

   return EXIT_SUCCESS;
}
