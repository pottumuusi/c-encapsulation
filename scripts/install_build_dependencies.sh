#!/bin/bash

set -e

readonly DEPENDENCIES_INSTALL_WORKAREA='/tmp/c-encapsulation/dependencies-install'

main() {
    local -r file_criterion_compressed='criterion-2.4.2-linux-x86_64.tar.xz'

    if [ ! -d "${DEPENDENCIES_INSTALL_WORKAREA}" ] ; then
        mkdir -p ${DEPENDENCIES_INSTALL_WORKAREA}
    fi

    pushd ${DEPENDENCIES_INSTALL_WORKAREA}

    wget https://github.com/Snaipe/Criterion/releases/download/v2.4.2/${file_criterion_compressed}
    tar -xvf ${file_criterion_compressed}

    pushd criterion-2.4.2
    echo "Copying headers and library under /usr/local/"
    sudo cp --verbose --recursive include/criterion /usr/local/include/
    sudo cp --verbose --no-dereference --preserve=links \
        lib/libcriterion* /usr/local/lib/
    popd # criterion-2.4.2

    popd # ${DEPENDENCIES_INSTALL_WORKAREA}

    rm --verbose --recursive ${DEPENDENCIES_INSTALL_WORKAREA}
}

main "${@}"
