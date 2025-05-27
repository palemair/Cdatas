/* handle a csvfile with C */
#define _GNU_SOURCE
#include <string.h>
#include <time.h>
#include "csv.h"
#include "iter.h"
#include "xtools.h"

/* turing machine parsing */
int parse_csv (char* datas, regexarray* rp, struct table** tb)
{
    typedef enum
    { ENDL = '\n', DQUOTE = '\"', OUTQ, INQ } position;
    const unsigned char SIZE = 64;
    uint32_t tsize = 2048;
    struct list** tmp = xmalloc (tsize * sizeof (struct list *));
    struct list** curr = tmp;

    *curr = init_list ();
    char* p = datas;
    int c;
    char* cell = xmalloc (SIZE);
    size_t count = 0;
    size_t nb = 1;
    char* pf = NULL;
    position state = OUTQ;
    int DELIM = (*tb)->delim;

    /* first line */
    struct list* first = init_list ();

    if ((*tb)->header)
    {
        while (((c = *p++) != ENDL) && (state == OUTQ))
        {
            if (count != 0 && ((count % (SIZE - 2)) == 0))
            {
                ++nb;
                cell = xreallocarray (cell, nb * SIZE, sizeof (char));
            }
            if (c == DQUOTE && state == OUTQ)
                state = INQ;

            else if (c == DQUOTE && state == INQ)
                state = OUTQ;

            else if (state == INQ && c == ENDL)
            {
                *(cell + count) = '\\';
                count++;
                *(cell + count) = 'n';
                count++;
                continue;
            }

            else if (state == OUTQ && (c == DELIM))
                /* field ended */
            {
                *(cell + count) = '\0';
                pf = xtrim (cell);

                int ret = append (&first, NULL, pf);

                if (ret == ERR)
                {
                    fprintf (stderr, "%s\n", "append error");
                }
                free (pf);

                count = 0;
                memset (cell, 0, nb * SIZE);
                continue;
            }
            *(cell + count) = c;
            count++;
        }
        /* end line */
        *(cell + count) = '\0';
        pf = xtrim (cell);

        int ret = append (&first, NULL, pf);

        if (ret == ERR)
        {
            fprintf (stderr, "%s\n", "append error");
        }
        free (pf);

        count = 0;
        memset (cell, 0, nb * SIZE);
    }

    /* following lines */
    while ((c = *p++))
    {
        if (count != 0 && ((count % (SIZE - 2)) == 0))
        {
            ++nb;
            cell = xreallocarray (cell, nb * SIZE, sizeof (char));
        }
        if (c == DQUOTE && state == OUTQ)
            state = INQ;

        else if (c == DQUOTE && state == INQ)
            state = OUTQ;

        else if (state == INQ && c == ENDL)
        {
            *(cell + count) = '\\';
            count++;
            *(cell + count) = 'n';
            count++;
            continue;
        }
        else if (state == OUTQ && ((c == DELIM) || (c == ENDL)))
            /* row or field ended */
        {
            *(cell + count) = '\0';
            pf = xtrim (cell);

            int ret = append (curr, rp, pf);

            if (ret == ERR)
            {
                fprintf (stderr, "%s\n", "append error");
            }
            free (pf);

            if (c == ENDL)
                /* row ended */
            {
                struct list* tmplist = *curr;
                struct field* tmpfd = tmplist->head;
                bool nilrow = true;

                /* check empty line */
                while (tmpfd)
                {
                    nilrow = (tmpfd->datatype != NIL) ? false : true;
                    if (nilrow == false) break;
                    tmpfd = tmpfd->nxt;
                }

                if (nilrow == false)
                {
                    if ((*tb)->height > (tsize - 2))
                    {
                        tsize <<= 1;
                        tmp = xreallocarray (tmp, tsize, sizeof (struct list *));
                        curr = (tmp + (*tb)->height + 1);
                    }
                    else
                    {
                        curr++;
                    }
                    *curr = init_list ();
                    (*tb)->height++;
                }
                else
                {
                    del_list (tmplist);
                }
            }
            count = 0;
            memset (cell, 0, nb * SIZE);
            continue;
        }
        *(cell + count) = c;
        count++;
    }
    free (*curr);
    free (cell);
    /* count free rows in the table */
    (*tb)->rowsremain = tsize - (*tb)->height;
    curr = (tmp + (*tb)->height + 1);
    *curr = NULL;

    (*tb)->t = tmp;
    (*tb)->width = (*tb)->t[0]->len;

    if ((*tb)->header == false)
    {
        int ret = 0;
        char* pts = NULL;

        for (unsigned int i = 1; i <= (*tb)->width; i++)
        {
            pts = baseA (i);
            ret = append (&first, NULL, pts);
            if (ret == ERR)
            {
                fprintf (stderr, "%s\n", "append error");
            }
        }
    }
    (*tb)->desc = first;
    update_tb_fd_width (*tb);

    return (EXIT_SUCCESS);
}

struct table* load_csv (char* filename, char delim, bool header, bool dataconvert)
{
    regexarray* rp = NULL;

    if (dataconvert)
    {
        rp = reg_init ();
    }

    char* buffer = xreadfile (filename);

    if (buffer == NULL)
    {
        fprintf (stderr, "%s", "error readfile\n");
        exit (EXIT_FAILURE);
    }

    struct table* tb = init_table (delim, header);
    int n = parse_csv (buffer, rp, &tb);

    if (n != EXIT_SUCCESS)
    {
        fprintf (stderr, "%s", "error parse_csv");
        return NULL;
    }
    if (rp != NULL)
    {
        free_reg (rp);
    }
    free (buffer);

    return tb;
}
