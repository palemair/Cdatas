/* some common functions */ 

#ifndef _TOOLS
#define _TOOLS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/* Macros */
#define START clock_t end,start = clock()
#define STOP end = clock()
#define TPS(str) printf("time for fnct : %s : %f \n",(str),(((double)end - start) / CLOCKS_PER_SEC))

#define ARRAYSIZE(p) (sizeof(p) / sizeof(p[0]))

/* malloc custom to avoid error test */
void* xmalloc (size_t size);
/* reallocarray custom to avoid error test */
void* xreallocarray (void* ptr, size_t nmemb, size_t size);
/* delete left and right space char + double quotes in a field */
char* xtrim (const char* raw);
/* load an entire file in a char buffer */
char* xreadfile (char* filename);

#endif
