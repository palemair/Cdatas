#ifndef _ITER
#define _ITER

#include "table.h"

typedef struct Listiter
{
    struct list *ls; //list
    struct field *curr; //current field
    int pos;
} *listiter;

typedef struct Tabiter
{
    struct table *tb; //table
    struct list *curr; //current list
    int pos;          
} *tabiter;

void init_listiter(listiter, struct list *);
void init_tabiter(tabiter, struct table *);
bool next_field(listiter);
bool next_list(tabiter);
void fprint_field(struct field* fd,FILE* outputfile,int lgnumber,int lgstr, int lgprecision);
void print_field(struct field* fd,int lgnumber,int lgstr,int lgprecision);
void clear_field (struct field* fd);
int set_field(struct field *fd, char* value, regexarray *rg);

#endif
