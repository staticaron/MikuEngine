#!/usr/bin/bash

rm -rf build

BUILD_TYPE="Release"

script_dir = $(cd "$(dirname "$0")" && pwd)"

bash "${script_dir}/build.sh"
