@echo off

cd ../

if not exist "build" (
    mkdir build
)

cmake -G "Ninja" -S . -B build 
cmake  --build build --target install

cd demos

if not exist "build" (
    mkdir build_demo
)

cmake -G "Ninja" -S . -B build

cmake --build build
