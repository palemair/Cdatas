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
void fprint_field(struct field* fd,FILE* outputfile, int fmtstr, int fmtprecis);

#endif
