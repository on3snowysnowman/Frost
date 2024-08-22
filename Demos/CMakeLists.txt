cmake_minimum_required(VERSION 3.24)

project(Frost_Demos)

# Set the paths for the lib and include files of the Frost Engine.
set(FROST_LIB_PATH C:/msys64/mingw64/lib)
set(FROST_INCLUDE_PATH C:/msys64/mingw64/include/Frost)

# Finds the Frost library on disk.
find_library(FROST_LIB NAMES Frost PATHS "${FROST_LIB_PATH}")

# Finds the required packages for Frost. nlohmann::json was found during the installation process
# since it is a purely header library.
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_executable(Demo demo.cpp)

set_target_properties(Demo PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

target_include_directories(Demo
PUBLIC
    ${FROST_INCLUDE_PATH}
    ${FROST_INCLUDE_PATH}/UITools
)

target_link_libraries(Demo
PRIVATE
    ${FROST_LIB}
    SDL2::SDL2
    SDL2_image
)

