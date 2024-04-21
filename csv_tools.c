/*
 * =====================================================================================
 *
 *       Filename:  csv_tools.c
 *
 *    Description:  different tools for csv extracting datas 
 *
 *        Version:  1.0
 *        Created:  12/04/2024 09:06:27
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

/* malloc custom to avoid error test */
void *xmalloc (size_t size)
{
   void *value = NULL;
   value = malloc (size);

   if (value == NULL)
   {
      perror ("Virtual memory exhausted");
      exit (EXIT_FAILURE);
   }

   return value;
}

/* reallocarray custom to avoid error test */
void *xreallocarray (void *ptr, size_t nmemb, size_t size)
{
   void *value = reallocarray (ptr, nmemb, size);
   if (value == 0)
   {
      perror ("Virtual memory exhausted");
      exit (EXIT_FAILURE);
   }
   return value;
}

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

/* delete left and right space char + double quotes in a field */
char *csvtrim (const char *raw)
{
      char *rawfield = strdupa (raw);
      char *begin = rawfield;
      char *end = rawmemchr (begin, '\0');
      if (end == NULL)
      {
         perror ("rawmemchr");
         return NULL;
      }

      while (isspace (*end) || *end == '\0' || *end == '"')
      {
         if (end-- == begin)
         {
            char p[] = "       (null)       ";
            return strdup(p);
         }
      }

      while (isspace (*begin) || *begin == '"')
         begin++;

      size_t len = ++end - begin;

      char scopy[len + 1];
      memcpy (scopy, begin, len);
      scopy[len] = '\0';

      return strdup (scopy);
}

struct list *init_list (void)
{
   struct list *new = xmalloc (sizeof (*new));
   new->head = NULL;
   new->tail = NULL;

   return new;
}

struct table *init_table (char delim, bool header)
{
   struct table *new = xmalloc (sizeof (*new));
   new->Dlim = delim;
   new->header = header;
   new->width = 0;
   new->height = 0;
   new->t = NULL;

   return new;
}

int append (struct list **ls, regexarray * rg, void *value)
{
   int Error_assign = 0;

   if (*ls == NULL)
   {
      fprintf (stderr, "%s\n", "Init struct list first !!");
      return 0;
   }
   else
   {
      struct field *new = xmalloc (sizeof (*new));
      new->nxt = NULL;
      new->datatype = typedata (rg, value);
      Error_assign = assign (new, value, new->datatype);

      if (Error_assign)
      {
         fprintf (stderr, "%s\n", "assignement error !!");
         exit (EXIT_FAILURE);
      }

      if ((*ls)->tail == NULL)
      {
         new->prv = (*ls)->tail;
         (*ls)->tail = new;
         (*ls)->head = new;

         return EXIT_SUCCESS;
      }
      else
      {
         new->prv = (*ls)->tail;
         (*ls)->tail->nxt = new;
         (*ls)->tail = new;
      }
   }
   return EXIT_SUCCESS;
}
