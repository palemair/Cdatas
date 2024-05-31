/* handle a csvfile with C */
#include <stdint.h>

struct field
{
    int value;
    struct field* nxt;
    struct field* prv;
};

struct list
{
    uint16_t len;
    struct field* head;
    struct field* tail;
};

struct list* init_list (void);
void drop_list (struct list* ls);


int append (struct list** ls, int v);
int appendleft (struct list** ls, int v);

int print (struct list* ls);
void pop (struct list** ls);
void popleft (struct list** ls);

bool is_start_head (struct list** ls, uint16_t index);

void del_by_index (struct list** ls, uint16_t index);
void insert_by_index (struct list** ls, uint16_t index, int value);

