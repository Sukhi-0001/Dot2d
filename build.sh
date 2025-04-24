#!/bin/bash
rm -f ./build/dot2d
mkdir -p build
cd build
cmake ..
mv compile_commands. json ..
make
echo "*********** PROGRAM EXECUTION **********"
./dot2d
