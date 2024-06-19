
if not exist "build" (
    mkdir build_demo
    cmake -G "Ninja" -S . -B build_demo
)

cmake --build build_demo