#!/usr/bin/env bash

source tools/travis-ci-functions.sh

test ./tools/generate-nvic-all.sh

test export PATH=$(pwd)/arm9/gcc-arm-none-eabi-9-2019-q4-major/bin:$PATH

mkdir -p build
pushd build
for TARGET_BOARD in $(ls ../src/target -I nucleo -I f)
do
    echob "selecting TARGET_BOARD: ${TARGET_BOARD}"
    for EXAMPLE in $(ls ../src/example)
    do
        echob "selecting EXAMPLE: ${EXAMPLE}"
        test cmake --configure -DTARGET_BOARD=${TARGET_BOARD} -DEXAMPLE=${EXAMPLE} ..
        test make -j$(nproc)
    done
done
popd
