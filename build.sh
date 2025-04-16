#!/bin/bash
mkdir -p build
cd build
cmake ..
mv compile_commands. json ..
make
echo "*********** PROGRAM EXECUTION **********"
./dot2d
