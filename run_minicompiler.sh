#!/usr/bin/env bash

set -euo pipefail

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
repo_root="$script_dir"

cd "$repo_root"

g++ -std=c++17 -Wall -Wextra -pedantic \
    MiniCompiler/main.cpp \
    MiniCompiler/lexer.cpp \
    MiniCompiler/parser.cpp \
    MiniCompiler/codegen.cpp \
    -o MiniCompiler/minicompiler

./MiniCompiler/minicompiler MiniCompiler/test_input.txt