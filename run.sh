#!/bin/bash

BUILD_DIR="build"

if [ ! -d "$BUILD_DIR" ]; then
    mkdir "$BUILD_DIR"
fi

cmake -S . -B "$BUILD_DIR"
cmake --build "$BUILD_DIR"

if [ $? -eq 0 ]; then
    cd "$BUILD_DIR"
    echo "--- Build finished successfully. Executing launch ---"
    ./launch
else
    echo "--- Error building ---"
    exit 1
fi

