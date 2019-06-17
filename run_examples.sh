#!/usr/bin/env bash
cd "$(dirname "$0")"
./build_all.sh

./simulation inputfiles/spec2.0.xml
./simulation_debug

echo done