#!/bin/bash

set -e

readonly DEPENDENCIES_INSTALL_WORKAREA='/tmp/c-encapsulation/dependencies-install'

main() {
    local -r file_criterion_compressed='criterion-v2.3.3-linux-x86_64.tar.bz2'

    if [ ! -d "${DEPENDENCIES_INSTALL_WORKAREA}" ] ; then
        mkdir -p ${DEPENDENCIES_INSTALL_WORKAREA}
    fi

    pushd ${DEPENDENCIES_INSTALL_WORKAREA}

    wget https://github.com/Snaipe/Criterion/releases/download/v2.3.3/${file_criterion_compressed}
    tar -xjvf ${file_criterion_compressed}

    pushd criterion-v2.3.3
    echo "Copying headers and library under /usr/local/"
    sudo cp --verbose --recursive include/criterion /usr/local/include/
    sudo cp --verbose lib/libcriterion* /usr/local/lib/
    popd # criterion-v2.3.3

    popd # ${DEPENDENCIES_INSTALL_WORKAREA}

    rm --verbose --recursive ${DEPENDENCIES_INSTALL_WORKAREA}
}

main "${@}"
