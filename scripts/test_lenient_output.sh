#!/bin/bash

set -e

cd $(dirname $0)

readonly C_ENCAPSULATION_BASE_PATH="$(pushd ../ &> /dev/null; pwd ; popd &> /dev/null)"

main() {
    local output=''

    pushd ${C_ENCAPSULATION_BASE_PATH}

    output="$(./out/lenient                                                 \
        | grep                                                              \
            -e "After calling demo_add_to_foo_unrecoverable, result is: 48" \
            -e "After calling demo_add_to_foo_recoverable, result is: 30"   \
        | wc --lines)"

    if [ "2" != "${output}" ] ; then
        echo "[!] Lenient encapsulation demo did not produce expected output."
        exit 1
    fi

    popd # ${C_ENCAPSULATION_BASE_PATH}
}

main "${@}"
