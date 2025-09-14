# Makefile
# Build void_scrappers_save_editor
# By J. Stuart McMurray
# Created 20250427
# Last Modified 20250428


# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic

# Program name and sources
PROG = void_scrappers_save_editor
SRCS = modify.c pop.c void_scrappers_save_editor.c

OBJS = $(SRCS:.c=.o)

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(PROG)
	prove -It --directives

clean:
	rm -f $(PROG) $(OBJS)
