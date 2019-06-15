#!/usr/bin/env bash
cd "$(dirname "$0")"
./build_all.sh

./simulation
./simulation_debug

echo done