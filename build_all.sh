#!/usr/bin/env bash
cd "$(dirname "$0")"
make -C engine >/dev/null 2>&1 &
cmake CMakeLists.txt
make
wait