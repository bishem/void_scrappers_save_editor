/*
 * pop.c
 * Pop a value off the savefile
 * By J. Stuart McMurray
 * Created 20250428
 * Last Modified 20250428
 */

#include <err.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pop.h"
#include "void_scrappers_save_editor.h"

/* pop_string returns a pointer to the next string in the len-length buf
 * starting at *start and advances start to the offset after the string. */
char *
pop_string(const char *buf, size_t len, size_t *start) {
        char     *ret;
        uint32_t  slen;

        /* Get the string's length. */
        POP(buf, len, *start, uint32_t, slen);

        /* Get the string itself. */
        ENSURE_SPACE(slen, len, *start);
        if (NULL == (ret = malloc(slen + 1)))
                err(15, "malloc");
        explicit_bzero(ret, slen + 1);
        memcpy(ret, (buf + *start), slen);
        *start += slen;

        return ret;
}

/* pop_number_key returns a numeric key, which is variable-length encoded.
 * Args are as for pop_string. */
char *
pop_number_key(const char *buf, size_t len, size_t *start)
{
        char     *ret;
        uint32_t  n;

        POP(buf, len, *start, uint32_t, n);
        if (-1 == asprintf(&ret, "%"PRIu32, n))
                err(14, "converting number to string");
        return ret;
}

/* pop_bool pops a byte and returns "true" or "false". */
const char *
pop_bool(const char *buf, size_t len, size_t *start)
{
        uint8_t b;
        POP(buf, len, *start, uint8_t, b);
        return b ? TRUE : FALSE;
}

/* pop_number pops a number, after dealing with the hairbrained alignemnt. */
double
pop_number(const char *buf, size_t len, size_t *start, size_t *vstart)
{
        double f;

        /* This is kinda kooky.  The number is a double, and it needs to be
         * float-aligned.  For obvious reasons.  Byte-popping should do it. */
        while (0 != (*start % 4)) {
                ENSURE_SPACE(1, len, *start);
                (*start)++;
        }
        *vstart = *start; /* This makes sense... */

        /* Granted, this'll break if float/double changes size. */
        f = 0.3;
        POP(buf, len, *start, double, f);
        return f;
}
