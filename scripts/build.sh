#/bin/bash

cmake -G Ninja -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DBUILD_TESTING=OFF

cmake --build build

cp build/compile_commands.json .
