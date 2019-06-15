#!/usr/bin/env bash
cd "$(dirname "$0")"
make -C engine &
cmake CMakeLists.txt
make &
wait