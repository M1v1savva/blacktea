#!/usr/bin/env bash

# Simple script to build hello_package
# Run from the project root:
# bash scripts/build.sh

mkdir -p build
cd build || exit 1
cmake ..
make