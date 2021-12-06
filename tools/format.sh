#!/bin/zsh

folder=src
local format_files=($(find "${folder}" -type f ! -path "src/stm*"))

clang-format --verbose -i ${format_files[@]}

