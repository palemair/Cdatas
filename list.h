/* header for csv_tools */

#ifndef LIST
#define LIST

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "regexarray.h"

/* MACROS */
#define DELSTRING(x) if (x->datatype == 0x00) free (x->strdata)

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

/* prototypes */
int assign (struct field* fd, char* value,regexarray* rg);

struct list *init_list (void);
int append_value (struct list** ls, regexarray* rg, void* value);
void del_list (struct list* ls);
void del_field_by_index (struct list** ls, uint16_t index);
bool is_start_head (struct list** ls, uint16_t index);
void popleft (struct list** ls);
void pop (struct list** ls);

#endif
