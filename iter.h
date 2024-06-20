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

extern void init_listiter(listiter, struct list *);
extern void init_tabiter(tabiter, struct table *);
extern bool next_field(listiter);
extern bool next_list(tabiter);
