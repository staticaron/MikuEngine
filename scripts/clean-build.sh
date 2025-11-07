#!/bin/bash

rm -rf build/

cmake -G Ninja -B build

cmake --build build/
