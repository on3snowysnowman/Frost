cmake_minimum_required(VERSION 3.24)

project(Frost_Tester)

# Path to the Frost Library on disk
set(FROST_PATH C:/Users/Joel/Frost)

# Find required libraries
find_library(FROST_LIB NAMES Frost PATHS "${FROST_PATH}/lib")
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

# Create executable
add_executable(Demo Demo.cpp)

# Set the output directory for the executable
set_target_properties(Demo PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})


# Include directories for Frost (if needed)
target_include_directories(Demo
PUBLIC
    ${FROST_PATH}/include
)

# Link Frost library to the executable
target_link_libraries(Demo
PRIVATE
    ${FROST_LIB}
    SDL2::SDL2
    SDL2_image
)
