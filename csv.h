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

/* Macros */
#define START clock_t end,start = clock()
#define STOP end = clock()
#define TPS printf("\ntemps écoulé : %f \n", (((double)end - start) / CLOCKS_PER_SEC))

#define ARRAYSIZE(p) (sizeof(p) / sizeof(p[0]))

/* globals */
typedef enum { OUTQ, INQ , DELIM = ',' , ENDL = '\n', DQUOTE = '\"'} position;
enum { LONG, FLOAT, TIME, DATE, PERCENT, STRING };

/* STRUCTURES */

typedef struct
{
unsigned char lgreg_list;
regex_t * reg_list;
} regexarray;





/* prototype */
void *xmalloc(size_t size);
void *xreallocarray(void *ptr, size_t nmemb,size_t size);
char *readfile(char* filename);
char* csvtrim(const char * raw);

#endif 
