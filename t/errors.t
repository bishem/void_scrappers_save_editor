#!/bin/bash
#
# errors.t
# Make sure we catch errors
# By J. Stuart McMurray
# Created 20250428
# Last Modified 20250428

set -euo pipefail

. t/shmore.subr

tap_plan 7

TD=t/testdata/errors

for HAVE in $TD/*.have; do
        GOT=$(./void_scrappers_save_editor $TD/orig $(<"$HAVE") 2>&1 ||:)
        WANT=$(< ${HAVE%%.have}.want)
        tap_is "$GOT" "$WANT" "Erorr correct - $HAVE"
done

# vim: ft=sh
