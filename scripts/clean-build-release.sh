#!/usr/bin/bash

rm -rf build

export BUILD_TYPE="Release"

ccache -C

export CCACHE_DISABLED=1

script_dir="$(cd "$(dirname "$0")" && pwd)"

bash "${script_dir}/build.sh"
