#!/usr/bin/bash

script_dir = $(cd "$(dirname "$0")" && pwd)"

bash "${script_dir}/build.sh"
bash "${script_dir}/run.sh"
