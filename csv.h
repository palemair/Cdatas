/* header for csv_tools */

#ifndef CSV_TOOLS
#define CSV_TOOLS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <regex.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

/* Macros */
#define START clock_t end,start = clock()
#define STOP end = clock()
#define TPS(str) printf("time for fnct : %s : %f \n",(str),(((double)end - start) / CLOCKS_PER_SEC))

#define ARRAYSIZE(p) (sizeof(p) / sizeof(p[0]))

/* globals */
enum { LONG, FLOAT, TIME, DATE, PERCENT, STRING, NIL };

/* STRUCTURES */
/* ~~~~~~~~~~~*/
/* regular expression vector */
typedef struct
{
   unsigned char lgreg_list;
   regex_t *reg_list;
} regexarray;

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
void *xmalloc (size_t size);
void *xreallocarray (void *ptr, size_t nmemb, size_t size);
char *readfile (char *filename);
char *csvtrim (const char *raw);
regexarray *reg_init (void);
void free_reg (regexarray * rp);

int typedata (regexarray * p, const char *strtest);
int assign (struct field *fd, const char *value, int datatype);

struct list *init_list (void);
struct table *init_table (char delim, bool header, int init_height);
int append (struct list **ls, regexarray * rg, void *value);

int load_csv (char *filename, char delim, bool header);
int parse_csv (char *datas, regexarray * rp, struct table **tb);

#endif
