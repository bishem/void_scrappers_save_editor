#!/bin/bash
#
# parse.t
# Make sure we can read a file ok
# By J. Stuart McMurray
# Created 20250428
# Last Modified 20250428

set -euo pipefail

. t/shmore.subr

tap_plan 2

GOTF=$(mktemp -t)
trap 'RET=$?; rm -f "$GOTF"; (exit $RET); tap_done_testing' EXIT

for HAVE in t/testdata/parse/*.have; do
        ./void_scrappers_save_editor "$HAVE" >"$GOTF"
        tap_ok "$?" "Parsed $HAVE ok" "$0" $LINENO

        WANT=${HAVE%%.have}.want
        DIFF=$(diff -u "$GOTF" "$WANT" ||:)
        tap_is "$DIFF" "" "$HAVE -> $WANT" "$0" $LINENO
done

# vim: ft=sh
