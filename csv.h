/* header for csv_tools */

#ifndef CSV_TOOLS
#define CSV_TOOLS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include "regexarray.h"
#include "xtools.h"

/* globals */
enum { LONG = 0x00, FLOAT = 0x01, TIME = 0x02, DATE= 0x04, PERCENT= 8, STRING=16, NIL=32 };
typedef enum { ENDL = '\n', DQUOTE = '\"', OUTQ, INQ } position;

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

int typedata (regexarray * p, const char *strtest);
int assign (struct field *fd, const char *value, int datatype);
int try_assign(struct field* f,void* value,regexarray* rg);

struct list *init_list (void);
struct table *init_table (char delim, bool header);
void del_list (struct list* ls);
void drop_table (struct table* tb);


int append (struct list **ls, regexarray * rg, void *value);

struct table* load_csv (char *filename, char delim, bool header);
int parse_csv (char *datas, regexarray * rp, struct table **tb);


#endif
