#!/usr/bin/bash

rm -rf build

script_dir="$(cd "$(dirname "$0")" && pwd)"

bash "${script_dir}/build.sh"
