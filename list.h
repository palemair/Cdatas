/* header for csv_tools */

#ifndef LIST
#define LIST

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "field.h"
#include "regexarray.h"

/* STRUCTURES */
/* ~~~~~~~~~~~*/
/* linked list for row */
struct list
{
   uint16_t len;
   struct field *head;
   struct field *tail;
};

/* prototypes */

struct list *init_list (void);
int append(struct list** ls, regexarray* rg, void* value);
void pop (struct list** ls);
void popleft (struct list** ls);
void del_list (struct list* ls);
void del_field_by_index (struct list** ls, uint16_t index);
bool is_start_head (struct list** ls, uint16_t index);

#endif
