/*
 * modify.c
 * Modify a value in the savefile
 * By J. Stuart McMurray
 * Created 20250428
 * Last Modified 20250428
 */

#include <err.h>
#include <stdio.h>
#include <string.h>

#include "void_scrappers_save_editor.h"

static void
modify_bool(char *buf, size_t len, size_t where, char val)
{
        /* Make sure we have a place to put this thing. */
        if ((where + sizeof(val)) > len)
                errx(21, "save file too small to change %zu-byte boolean at "
                        "offset 0x%zx", sizeof(val), where);
        buf[where] = val;
}

/* modify modifie buf according to mod, which should be of the form 0xoff=val,
 * where val is either true, false, or a float. */
void
modify(char *buf, size_t len, const char *mod)
{
        const char *valp;
        int         ret;
        ssize_t     where;
        double      d;

        /* Work out where we're sticking our value. */
        switch (ret = sscanf(mod, "%zi", &where)) {
                case 1: /* Good. */
                        break;
                case 0:
                        errx(16, "no offset in %s", mod);
                        break;
                case EOF:
                        errx(17, "failed to parse %s", mod);
                        break;
                default:
                        errx(18, "unexpectedly got %d offsets from %s", ret,
                                mod);
                        break;
        }
        if (0 >= where)
                errx(19, "offset %zi in %s must be greater than zero", where,
                        mod);

        /* Work out what we're sticking there. */
        if (NULL == (valp = strchr(mod, '=')))
                errx(20, "missing = in %s", mod);
        valp++;
        if ('\0' == *valp)
                errx(25, "missing value in %s", mod);

        /* Bools are easy. */
        if (0 == strcmp(valp, TRUE)) {
                modify_bool(buf, len, where, 1);
                return;
        } else if (0 == strcmp(valp, FALSE)) {
                modify_bool(buf, len, where, 0);
                return;
        }

        /* Floats take work. */
        switch (ret = sscanf(valp, "%lf", &d)) {
                case 1: /* Good. */
                        break;
                case 0:
                        errx(22, "failed to read double from %s", valp);
                        break;
                case EOF:
                        errx(23, "failed to parse %s", valp);
                        break;
                default:
                        errx(24, "unexpectedly got %d values from %s", ret,
                                valp);
                        break;
        }
        if ((where + sizeof(d)) > len)
                errx(25, "save file too small to change %zu-byte number at "
                        "offset 0x%zx", sizeof(d), where);
        memcpy(buf + where, &d, sizeof(d));
}
