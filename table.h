/* header for csv_tools */

#ifndef TABLE
#define TABLE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


/* STRUCTURES */
/* ~~~~~~~~~~~*/
/* simple csv field */
struct field
{
   uint8_t datatype;
   union
   {
      long lgdata;
      double dbdata;
      char *strdata;
   };
   struct field *nxt;
   struct field *prv;
};

/* linked list for row */
struct list
{
   uint16_t len;
   struct field *head;
   struct field *tail;
};

/* table structure containing csv */
struct table
{
   struct list **t;
   char Dlim;
   bool header;
   uint16_t width;
   uint32_t height;
};

/* prototypes */

struct list *init_list (void);
struct table *init_table (char delim, bool header);
void del_list (struct list* ls);
void drop_table (struct table* tb);
void del_field_by_index (struct list** ls, uint16_t index);
bool is_start_head (struct list** ls, uint16_t index);
void popleft (struct list** ls);
void pop (struct list** ls);

#endif
