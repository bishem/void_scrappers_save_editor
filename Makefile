# Makefile
# Build void_scrappers_save_editor
# By J. Stuart McMurray
# Created 20250427
# Last Modified 20250428

COPTS      = -Wall --pedantic -Wextra -Werror
NOMAN      = noman
PROG       = void_scrappers_save_editor
SRCS       = modify.c pop.c void_scrappers_save_editor.c

test: ${PROG}
	prove -It --directives

.include <bsd.prog.mk>
