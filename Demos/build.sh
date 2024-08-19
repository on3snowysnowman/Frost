#!/bin/bash

# Specify the directory path for the demo build.
DEMO_BUILD_DIRECTORY="build_demo"

# Specify the directory path for the Engine bild.
ENGINE_BUILD_DIRECTORY="build"


# Navigate to the Engine's directory.
cd ..

if [ ! -d $ENGINE_BUILD_DIRECTORY ]; then

    echo "$ENGINE_BUILD_DIRECTORY does not exist."
    mkdir "$ENGINE_BUILD_DIRECTORY"
    echo "Created directory"
fi


echo "Configuring Engine build."

# Configure cmake for the Engine build.
cmake -B $ENGINE_BUILD_DIRECTORY -S . -G "Ninja"

echo "Building Engine."

# Build Engine
cmake --build $ENGINE_BUILD_DIRECTORY --target install


# Navigate to the Demo's directory.
cd Demos

if [ ! -d $DEMO_BUILD_DIRECTORY ]; then

    echo "$DEMO_BUILD_DIRECTORY does not exist."
    mkdir "$DEMO_BUILD_DIRECTORY"
    echo "Created directory"
fi

echo "Configuring Demo build."

# Configure cmake for the Engine build.
cmake -B $DEMO_BUILD_DIRECTORY -S . -G "Ninja"

echo "Building Demo."

# Build Engine
cmake --build $DEMO_BUILD_DIRECTORY 