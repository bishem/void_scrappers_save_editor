#!/bin/ksh
#
# basic_tests.t
# Make sure our code is up-to-date and doesn't have debug things.
# By J. Stuart McMurray
# Created 20250427
# Last Modified 20250428

set -uo pipefail

. t/shmore.subr

tap_plan 4

# Make sure we didn't leave any stray DEBUGs or TAP_TODOs lying about.
GOT=$(egrep -InR '(#|\*)[[:space:]]*()DEBUG' | sort -u)
tap_is "$GOT" "" "No files with DEBUG comments" "$0" $LINENO
GOT=$(egrep -InR 'TAP_TODO[=]' t/*.t | sort -u)
tap_is "$GOT" "" "No TAP_TODO's" "$0" $LINENO
GOT=$(egrep -In  'TODO():' *.h *.c t/*.t | sort -u)
tap_is "$GOT" "" "No TODO's" "$0" $LINENO

# Make sure our #include's are sorted.
subtest() {
        for F in *.c *.h; do
                SYS=$(egrep '^#include <' "$F")
                GOT=$(echo -E "$SYS" | egrep '#include <sys/')
                WANT=$(echo -E "$GOT" | sort -u)
                tap_is "$GOT" "$WANT" "<sys/includes> in $F alphabetical"
                GOT=$(echo -E "$SYS" | egrep -v '#include <sys/')
                WANT=$(echo -E "$GOT" | sort -u)
                tap_is "$GOT" "$WANT" "<non-sys/includes> in $F alphabetical"
                GOT=$(egrep '#include "' "$F")
                WANT=$(echo -E "$GOT" | sort -u)
                tap_is "$GOT" "$WANT" "\"includes\" in $F alphabetical"
        done
}
tap_subtest "Includes are alphabetized" "subtest" "$0" $LINENO

# vim: ft=sh
