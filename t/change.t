#!/bin/bash
#
# change.t
# Make sure we can change a file ok
# By J. Stuart McMurray
# Created 20250428
# Last Modified 20250428

set -euo pipefail

. t/shmore.subr

tap_plan 9

TD=t/testdata/change
MODF=$(mktemp -t)
GOTF=$(mktemp -t)
CHKF=$(mktemp -t)
trap 'RET=$?; rm -f "$GOTF" "$MODF" "$CHKF";
        (exit $RET); tap_done_testing' EXIT

# Make sure our parsed original file is correct
./void_scrappers_save_editor $TD/orig >"$CHKF"
GOT=$(diff "$TD/orig.parsed" "$CHKF" ||:)
tap_is "$GOT" "" "Original parsed file ($TD/orig.parsed) correct"

for HAVE in $TD/*.have; do
        # Modify a fresh copy of the savefile
        cp $TD/orig "$MODF"
        ./void_scrappers_save_editor "$MODF" $(<$HAVE) >"$GOTF"

        # Make sure the modifications stuck.
        ./void_scrappers_save_editor "$MODF" >"$CHKF"
        GOT=$(diff "$GOTF" "$CHKF" ||:)
        tap_is "$GOT" "" "Modifications stuck - $HAVE" "$0" $LINENO

        # Make sure the modifications were as expected.
        GOT=$(diff $TD/orig.parsed "$GOTF" ||:)
        tap_is "$GOT" "$(<"${HAVE%%.have}.want")" \
                "Modifications correct - $HAVE" \
                "$0" $LINENO
done


# vim: ft=sh
