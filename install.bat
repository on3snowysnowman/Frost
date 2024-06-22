@echo off

if not exist "build" (
    mkdir build
)

cmake -G "Ninja" -S . -B build

cmake --build build --target install


