/* Handle a csvfile with C */
#define _GNU_SOURCE
#include <locale.h>
#include "csv.h"
#include "print.h"
#include "xtools.h"
#include "iter.h"

int main ()
{
    if(setlocale(LC_CTYPE | LC_COLLATE,"") == NULL)
    {
        perror("setlocale");
        return EXIT_FAILURE;
    }
    
    START;
    struct table* tb = load_csv ("CSV-file/technic.csv", ',', true,true);
    STOP;
    
    TPS ("load_csv");

    tail(tb,20);

    drop_table (tb);
    
    return EXIT_SUCCESS;
}
