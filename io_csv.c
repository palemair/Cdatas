/* handle a csvfile with C */
#define _GNU_SOURCE
#include "csv.h"

/* load an entire file in a char buffer */
char *readfile (char *filename)
{
   int fd = 0;
   char *buffer = NULL;

   if ((fd = open (filename, O_RDONLY, 0)) != -1)
   {
      off_t lg = lseek (fd, 0, SEEK_END);
      lseek (fd, 0, SEEK_SET);

      if (lg > 0)
      {
         buffer = xmalloc (lg + 1);
         memset (buffer, 0, lg + 1);
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

int parse_csv (char *datas, regexarray * rp, struct table **tb)
{
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
         free (pf);
         
         if (c == ENDL)
         {
            max_width = (curr_width >= max_width) ? curr_width : max_width;

            if((*tb)->height > (tsize - 2))
            {
                /* k++; */
                tmp = xreallocarray(tmp, 2 * tsize,sizeof(struct list*));
                tsize <<= 1;
                curr = (tmp + (*tb)->height + 1);
            }
            else
            {
                curr++;
            }
            *curr = init_list ();
            (*tb)->height++;
            curr_width = 0;
         }
         count = 0;
         memset (cell, 0, nb * SIZE);
         continue;
      }

      *(cell + count) = c;
      count++;
   }

   free (*curr);
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
   int n = parse_csv (buffer, rp, &tb);
   if(n != EXIT_SUCCESS)
   {
       fprintf(stderr,"%s","error parse_csv");
       return NULL;
   }

   free_reg (rp);
   free (buffer);

   return tb;
}

void clear_row(struct list *ls)
{
   struct field *tmp;
   struct field *pelem = ls->head;
   while(pelem)
   {
     tmp = pelem;
     if(tmp->datatype & STRING) 
     {
        free(tmp->strdata);
     }
     pelem = pelem->nxt;
     free(tmp);
   }
   ls->head = NULL;
   ls->tail = NULL;
}

void drop_table(struct table *tb)
{
   for (int u = 0; u < (int) tb->height; u++)
   {
       clear_row(tb->t[u]);
       free(tb->t[u]);
   }
   free(tb->t);
   free(tb);
}

int main ()
{
   START;
   struct table *tb = load_csv("CSV-file/technic.csv",',',false);

   STOP;
   TPS ("load_csv");

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
               printf ("%20.20s ", "---");
               break;
            }
         case STRING:
            {
               printf ("%-25.25s ", f->strdata);
               break;
            }
         }
         f = f->nxt;
      }
      printf ("\n");
   }

drop_table(tb);

   return EXIT_SUCCESS;
}
