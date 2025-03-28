#!/bin/bash

set -e

cd $(dirname $0)

readonly C_ENCAPSULATION_BASE_PATH="$(pushd ../ &> /dev/null; pwd ; popd &> /dev/null)"

main() {
    local -r executable_lenient='./out/lenient'
    local matched_line_count=''
    local total_line_count=''

    pushd ${C_ENCAPSULATION_BASE_PATH}

    if [ ! -f "${executable_lenient}" ] ; then
        echo "[!] Executable ${executable_lenient} not found."
        exit 1
    fi

    matched_line_count="$(${executable_lenient}                             \
        | grep                                                              \
            -e "After calling demo_add_to_foo_unrecoverable, result is: 48" \
            -e "After calling demo_add_to_foo_recoverable, result is: 30"   \
        | wc --lines)"

    total_line_count="$(${executable_lenient} | wc --lines)"

    if [ "2" != "${matched_line_count}" ] ; then
        echo "[!] Failed to match lenient encapsulation demo output."
        exit 1
    fi

    if [ "2" != "${total_line_count}" ] ; then
        echo "[!] Unexpected line amount in lenient encapsulation demo output."
        exit 1
    fi

    popd # ${C_ENCAPSULATION_BASE_PATH}
}

main "${@}"
