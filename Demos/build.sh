#!/bin/bash

# Specify the build directory.
BUILD_DIRECTORY="build"


# Navigate to the Engine's directory.
cd ..

# If the build directory does not exist.
if [ ! -d $BUILD_DIRECTORY ]; then

    echo "$BUILD_DIRECTORY does not exist."
    mkdir "$BUILD_DIRECTORY"
    echo "Created directory"
fi


echo "Configuring Engine build."

# Configure cmake for the Engine build.
cmake -B $BUILD_DIRECTORY -S . -G "Ninja"

echo "Building Engine."

# Build Engine
cmake --build $BUILD_DIRECTORY --target install


# Navigate to the Demo's directory.
cd Demos

# If the build directory does not exist.
if [ ! -d $BUILD_DIRECTORY ]; then

    echo "$BUILD_DIRECTORY does not exist."
    mkdir "$BUILD_DIRECTORY"
    echo "Created directory"
fi

echo "Configuring Demo build."

# Configure cmake for the Engine build.
cmake -B $BUILD_DIRECTORY -S . -G "Ninja"

echo "Building Demo."

# Build Engine
cmake --build $BUILD_DIRECTORY 