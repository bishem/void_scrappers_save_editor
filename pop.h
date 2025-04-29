/*
 * pop.h
 * Pop a value off the savefile
 * By J. Stuart McMurray
 * Created 20250428
 * Last Modified 20250428
 */

#ifndef HAVE_POP_H
#define HAVE_POP_H 1

#include <err.h>

/* ENSURE_SPACE ensures there's n bytes left in a len byte buffer starting at
 * start. */
#define ENSURE_SPACE(n, len, start) do {          \
        if (((len) - (start)) < (n))              \
                errx(13, "savefile seems truncated"); \
} while (0) 

/* POP ensures there's enough space left in buf of length len starting at
 * start for v, pops enough bytes from the table for v of type T, and advances
 * start by that number of bytes. */
#define POP(buf, len, start, T, v) do {      \
        ENSURE_SPACE(sizeof(T), len, start); \
        v = *(T *)(buf + start);             \
        (start) += sizeof(T);                \
} while (0)

char       *pop_string(const char *buf, size_t len, size_t *start);
char       *pop_number_key(const char *buf, size_t len, size_t *start);
const char *pop_bool(const char *buf, size_t len, size_t *start);
double      pop_number(const char *buf, size_t len, size_t *start,
                size_t *vstart);

#endif /* #ifndef HAVE_POP_H */
