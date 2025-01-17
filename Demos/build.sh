#!/bin/bash

# Specify the build directory.
BUILD_DIRECTORY="build"

# Navigate to source directory. 
cd .. 

# Run Engine's build script.
./build.sh

# Navigate to the Demo's directory.
cd Demos

# If the build directory does not exist.
if [ ! -d $BUILD_DIRECTORY ]; then

    echo "Directory \"$BUILD_DIRECTORY\" does not exist."
    mkdir "$BUILD_DIRECTORY"
    chmod 777 $BUILD_DIRECTORY
    echo "Created directory"
fi

echo "Configuring Demo build."

# Configure cmake for the Engine build.
cmake -B $BUILD_DIRECTORY -S . -G "Ninja"
# cmake -DCMAKE_BUILD_TYPE=Release  -B $BUILD_DIRECTORY -S . -G "Ninja"

echo "Building Demo."

# Build Engine
cmake --build $BUILD_DIRECTORY 

sudo chmod 777 -R assets 
sudo chmod 777 -R data
