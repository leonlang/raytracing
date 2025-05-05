#!/bin/bash

echo "==== Step 1: Creating build directory ===="
mkdir -p build
cd build || { echo "Failed to navigate to build directory"; exit 1; }

echo "==== Step 2: Running CMake configuration ===="
cmake ..
if [ $? -ne 0 ]; then
    echo "CMake configuration failed!"
    exit 1
fi

echo "==== Step 3: Building the project ===="
cmake --build .
if [ $? -ne 0 ]; then
    echo "Build failed!"
    exit 1
fi

echo "==== Step 4: Executing the program ===="
cd ../
./build/main
if [ $? -ne 0 ]; then
    echo "Program execution failed!"
    exit 1
fi

echo "==== All steps completed successfully! ===="
