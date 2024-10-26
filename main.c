/* Handle a csvfile with C */
#define _GNU_SOURCE
#include <locale.h>
#include "csv.h"
#include "print.h"
#include "xtools.h"
#include "iter.h"

int main ()
{
    if (setlocale (LC_CTYPE | LC_COLLATE, "") == NULL)
    {
        perror ("setlocale");
        return EXIT_FAILURE;
    }

    START;
    struct table* tb = load_csv ("CSV-file/technic.csv", ',', true, true);
    STOP;
    TPS ("load_csv");

    print_header (tb);
    printline (tb, 1);
    printline (tb, 5);
    printlines (tb, 5, 12);
    r_printable (tb, 5);

    drop_table (tb);

    struct table* t = load_csv ("CSV-file/technic.csv", ',', false, true);

    head (t, 10);
    tail (t, 10);
    drop_table (t);

    return EXIT_SUCCESS;
}
