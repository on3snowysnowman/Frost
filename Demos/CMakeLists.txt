
cmake_minimum_required(VERSION 3.24)

project(Frost_Demos)

# Path to the Frost Library on disk. This should be the same path as the one specified in the 
# "Frost_Path.txt" file.
set(FROST_PATH C:/Users/on3sn/Frost)

# Finds the Frost library on disk, this line does not need to be modified.  
find_library(FROST_LIB NAMES Frost PATHS "${FROST_PATH}/lib")

# Finds the required packages for Frost. nlohmann::json was found during the installation process
# since it is a purely header library.
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_executable(Generic_Tester generic_tester.cpp)

set_target_properties(Generic_Tester PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

target_include_directories(Generic_Tester
PUBLIC
    ${FROST_PATH}/include/Frost
    ${FROST_PATH}/include/Frost/UITools
)

target_link_libraries(Generic_Tester
PRIVATE
    ${FROST_LIB}
    SDL2::SDL2
    SDL2_image
)
