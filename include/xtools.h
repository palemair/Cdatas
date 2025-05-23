/* some common functions */ 

#ifndef _TOOLS
#define _TOOLS

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

/* Macros */
#define START clock_t end,start = clock()
#define STOP end = clock()
#define TPS(str) printf("time for fnct %s : %f \n",(str),(((double)end - start) / CLOCKS_PER_SEC))

#define ARRAYSIZE(p) (sizeof(p) / sizeof(p[0]))

/* malloc custom to avoid error test */
void* xmalloc (size_t size);
/* calloc custom to avoid error test */
void* xcalloc (size_t elemcount, size_t elemsize);
/* reallocarray custom to avoid error test */
void* xreallocarray (void* ptr, size_t nmemb, size_t size);
/* delete left and right space char + double quotes in a field */
char* xtrim (const char* raw);
/* load an entire file in a char buffer */
char* xreadfile (char* filename);
/* converter base 10 to 26A */
char *baseA(unsigned int nb);
/* input one line on stdin */
char *input(char *string);

#endif
