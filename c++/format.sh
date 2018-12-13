#!/bin/bash

# Format C++ files as LLVM style with bigger indent.

clang-format-4.0 -style="{BasedOnStyle: llvm, IndentWidth: 4}" $1 



