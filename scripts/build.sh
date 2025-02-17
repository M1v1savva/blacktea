#!/usr/bin/env bash

# Simple script to build the hello_package
# Run from the project root:
# ./scripts/build.sh

mkdir -p build
cd build || exit 1
cmake ..
make