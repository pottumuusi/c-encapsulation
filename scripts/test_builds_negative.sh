#!/bin/bash

set -e

cd $(dirname $0)

readonly C_ENCAPSULATION_BASE_PATH="$(pushd ../ &> /dev/null; pwd ; popd &> /dev/null)"

main() {
    local latest_exit=0

    pushd ${C_ENCAPSULATION_BASE_PATH}

    echo "Building lenient encapsulation code."
    make lenient_private_via_public_interface || latest_exit=${?}
    if [ "0" == "${latest_exit}" ] ; then
        echo ""
        echo "Unexpectedly managed to build lenient encapsulation code, where "
        echo "private data is accessed via public interface. Exiting with "
        echo "error."
        echo ""
        exit 1
    fi

    popd # ${C_ENCAPSULATION_BASE_PATH}

    echo "Experienced build failures where expected, exiting with success!"

    exit 0
}

main "${@}"
