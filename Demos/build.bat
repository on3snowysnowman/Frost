@echo off

cd ../

if not exist "build" (
    mkdir build
    cmake -G "Ninja" -S . -B build
)

cmake --build build --target install

cd demos

if not exist "build_demo" (
    mkdir build_demo
)

cmake -G "Ninja" -S . -B build_demo

cmake --build build_demo
