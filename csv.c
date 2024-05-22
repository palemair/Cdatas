/* handle a csvfile with C */
#define _GNU_SOURCE
#include "csv.h"

int append_value (struct list** ls, regexarray* rg, void* value)
{
    int Error_assign = -1;

    if (*ls == NULL)
    {
        fprintf (stderr, "%s\n", "Init struct list first !!");
    }
    else
    {
        struct field* new = xmalloc (sizeof (*new));
        Error_assign = try_assign (new, value, rg);

        if (Error_assign == -1)
        {
            fprintf (stderr, "%s\n", "assignement error !!");
            return Error_assign;
        }

        if ((*ls)->tail == NULL)
        {
            new->prv = (*ls)->tail;
            (*ls)->tail = new;
            (*ls)->head = new;
        }
        else
        {
            new->prv = (*ls)->tail;
            (*ls)->tail->nxt = new;
            (*ls)->tail = new;
        }
        (*ls)->len++;
    }
    return Error_assign;
}

int try_assign(struct field* f,void* value,regexarray* rg)
{
        f->nxt = NULL;
        f->datatype = typedata (rg, value);

        return assign (f, value);
}

int assign (struct field* fd, const char* value)
{
    int nbconv = 0;
    switch (fd->datatype)
    {
    case LONG:
        {
            long result = 0;

            nbconv = sscanf (value, "%ld", &result);
            if (nbconv != 1)
            {
                fprintf (stderr, "%s\n", "long conversion fail !!");
                return -1;
            }
            else
            {
                fd->lgdata = result;
                return LONG;
            }
            break;
        }
    case FLOAT:
    case PERCENT:
        {
            double result = 0.0;

            errno = 0;
            result = strtod (value, NULL);
            if (errno != 0)
            {
                perror ("strtod");
                return -1;
            }
            else
            {
                fd->dbdata = result;
                return FLOAT;
            }
            break;
        }
    case TIME:
    case DATE:
    case NIL:
        {
            fd->strdata = NULL;
            return NIL;
            break;
        }
    case STRING:
        {
            if (NULL == (fd->strdata = strdup (value)))
            {
                perror ("strdup");
                return (EXIT_FAILURE);
            }
            else
            {
                return STRING;
            }
            break;
        }
    default:
        {
            return -1;
            break;
        }
    }
}


/* turing machine parsing */
int parse_csv (char* datas, regexarray * rp, struct table** tb)
{
    typedef enum { ENDL = '\n', DQUOTE = '\"', OUTQ, INQ } position;
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
    int DELIM = (*tb)->Dlim;
    
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
            int err = append_value (curr, rp, pf);
            if (err == -1)
            {
                fprintf (stderr, "%s\n", "append error");
            }
            free (pf);

            if (c == ENDL)
            /* row ended */
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
            count = 0;
            memset (cell, 0, nb * SIZE);
            continue;
        }
        *(cell + count) = c;
        count++;
    }
    free (*curr);
    free (cell);
    tmp = xreallocarray (tmp, (*tb)->height, sizeof (struct list *));
    (*tb)->t = tmp;
    (*tb)->width = (*tb)->t[0]->len;

    return (EXIT_SUCCESS);
}

struct table* load_csv (char* filename, char delim, bool header)
{
    regexarray* rp = reg_init ();
    char* buffer = xreadfile (filename);
    if (buffer == NULL)
    {
        fprintf (stderr, "%s", "error readfile\n");
        exit(EXIT_FAILURE);
    }

    struct table* tb = init_table (delim, header);
    int n = parse_csv (buffer, rp, &tb);
    if (n != EXIT_SUCCESS)
    {
        fprintf (stderr, "%s", "error parse_csv");
        return NULL;
    }
    free_reg (rp);
    free (buffer);

    return tb;
}
