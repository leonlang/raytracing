@echo off

REM Create build directory
if not exist build mkdir build
cd build

REM Run CMake configuration
cmake ..

REM Build the project
cmake --build .

REM Execute the program
main.exe
