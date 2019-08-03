#!/usr/bin/env bash

source tools/travis-ci-functions.sh

test tar -cf stm32-boilerplate-examples.tar build/bin/*
