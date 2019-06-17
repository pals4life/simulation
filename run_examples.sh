#!/usr/bin/env bash
cd "$(dirname "$0")"
./build_all.sh

./simulation inputfiles/spec1.0.xml
./simulation inputfiles/spec2.0.xml
./simulation_debug

for file in inputfiles/use_case_test_files ; do
    ./simulation $file
done


echo done