#!/bin/bash

rm -rf build

cmake -G Ninja -B build -DCMAKE_BUILD_TYPE=Release

cmake --build build

cp build/compile_commands.json .
