#!/usr/bin/env bash
cd "$(dirname "$0")"
./build_all.sh

run(){
    echo
    echo "###################################################"
    echo $1
    echo "###################################################"
    echo
    sleep 1
    ./simulation $1
}

run inputfiles/spec1.0.xml
run inputfiles/spec2.0.xml

echo
echo "###################################################"
echo tests
echo "###################################################"
echo
sleep 1
./simulation_debug

run inputfiles/extra_examples/full_simulation.xml

for file in inputfiles/use_case_test_files/* ; do
    run $file
done


echo done