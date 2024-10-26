/* header for csv_tools */
#ifndef _FIELD
#define _FIELD

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "regexarray.h"

/* STRUCTURES */
/* ~~~~~~~~~~~*/
/* simple csv field */
struct field
{
   uint16_t fdlenth;
   uint16_t datatype;
   union
   {
      long long lgdata;
      double dbdata;
      char *strdata;
   };
   struct field *nxt;
   struct field *prv;
};

/* PROTOTYPES */
int set_field (struct field* fd, char* value, regexarray* rg);
void clear_field (struct field* fd);
void print_field(const struct field* fd, int fdwidth, int fdprecis);
void fprint_field(const struct field* fd,FILE* outputfile, int fdwidth, int fdprecis);

#endif
