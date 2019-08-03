#!/usr/bin/env bash

source tools/travis-ci-functions.sh

test tools/clang-format.sh
test git diff --quiet --ignore-submodules
test tools/install-arm-toolchain.sh
test tools/build-all.sh
test tools/package-deployment.sh
