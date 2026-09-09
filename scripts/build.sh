#/usr/bin/bash

test -z "$BUILD_TYPE" && BUILD_TYPE="Debug"

echo "Building in ${BUILD_TYPE} mode."

cmake -G Ninja -B build -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DBUILD_TESTING=OFF

cmake --build build

cp build/compile_commands.json .
