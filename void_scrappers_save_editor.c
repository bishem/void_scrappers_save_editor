/*
 * void_scrappers_save_editor.c
 * Savegame editor for Void Scrappers
 * By J. Stuart McMurray
 * Created 20250427
 * Last Modified 20250428
 */

#include <sys/mman.h>

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "luaish.h"
#include "modify.h"
#include "pop.h"
#include "script_tableish.h"
#include "void_scrappers_save_editor.h"

/* IFSET evaluates to x if we're setting memory in the savefile, y otherwise. */
#define IFSET(x, y) (willset ? (x) : (y))
int willset;

/* Our own constants. */
#define TRUE    "true"
#define FALSE   "false"

void
usage(void)
{
        fprintf(stderr, "usage: %s savefile [offset=value...]\n",
                getprogname());
        exit(1);
}

/* data_start checks that buf starts with the proper header and returns the
 * offset of the saved data.  It terminates the program on error. */
off_t
data_start(const char *buf, size_t len)
{
        uint32_t v;
        size_t skip = 0;

        /* Make sure table starts with the proper magic. */
        if (len < sizeof(v))
                errx(10, "savefile too small for 4-byte magic");
        v = *(uint32_t *)buf;
        if (MAGIC != v)
                errx(8, "incorrect magic number at start of table");
        skip += sizeof(v);

        /* Make sure version is current. */
        if (len < sizeof(v))
                errx(7, "savefile too small for 4-byte version");
        v = *(uint32_t *)(buf + skip);
        if (FORMAT_VERSION != v)
                errx(9, "savefile version is %d, not %d", v, FORMAT_VERSION);
        skip += sizeof(v);

        return skip;
}

/* parse_table parses and prints the next table in buf, starting at start with
 * nt tabs before each line and containing size elements.  It terminates the
 * program on error. */
void
parse_table(const char *buf, size_t len, size_t *start, uint32_t size, int nt)
{
        char      key_type, value_type;
        char     *key;
        double    d;
        int       j;
        size_t    vstart;
        uint32_t  i, count;

        /* Print ALL the elements. */
        for (i = 0; i < size; i++) {
                /* Grab the key and value types. */
                POP(buf, len, *start, char, key_type);
                POP(buf, len, *start, char, value_type);

                switch (key_type) {
                        case LUA_TSTRING:
                                key = pop_string(buf, len, start);
                                break;
                        case LUA_TNUMBER:
                                key = pop_number_key(buf, len, start);
                                break;
                        default:
                                errx(11, "unknown key type %d", key_type);
                                break; /* For just in case. */
                }

                for (j = 0; j < nt; ++j)
                        printf("        ");
                printf("%s", key);

                free(key); key = NULL;
                vstart = *start;

                switch (value_type) {
                        case LUA_TBOOLEAN:
                                printf("\t0x%04zx=%s\n", vstart, 
                                        pop_bool(buf, len, start));
                                break;
                        case LUA_TNUMBER:
                                d = pop_number(buf, len, start, &vstart);
                                printf("\t0x%04zx=%f\n", vstart, d);
                                break;
                        case LUA_TTABLE:
                                POP(buf, len, *start, uint32_t, count);
                                printf(" (%d)\n", count);
                                parse_table(buf, len, start, count, nt+1);
                                break;
                        default:
                                errx(12, "unknown value type %d", value_type);
                                break; /* For just in case. */
                }
        }
}

int
main(int argc, char **argv)
{
        char     *buf;
        int       i, fd;
        off_t     end;
        size_t    len, start;
        uint32_t  count;

        /* Get file and things to set. */
        if (2 > argc)
                errx(2, "need a savefile");
        willset = NULL != argv[2];

        unveil(argv[1], IFSET("rw", "r"));
        if (willset)
                pledge("stdio rpath wpath", "");
        else
                pledge("stdio rpath", "");

        /* Get hold of the file. */
        if (-1 == (fd = open(argv[1], IFSET(O_RDWR, O_RDONLY))))
                err(3, "open %s", argv[1]);
        if (-1 == (end = lseek(fd, 0, SEEK_END)))
                err(4, "lseek");
        len = (size_t)end;
        if (MAP_FAILED == (buf = mmap(NULL, len,
                IFSET(PROT_READ | PROT_WRITE, PROT_READ), MAP_SHARED, fd, 0)))
                err(5, "mmap");
        if (-1 == close(fd))
                err(6, "close");

        pledge("stdio", "");

        /* Make the changes requested. */
        for (i = 2; i < argc; ++i)
                modify(buf, len, argv[i]);

        /* Work out where the real data starts. */
        start = data_start(buf, len);

        /* Print ALL the things! */
        POP(buf, len, start, uint32_t, count);
        parse_table(buf, len, &start, count, 0);

        return 0;
}
