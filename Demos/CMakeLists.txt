cmake_minimum_required(VERSION 3.24)

project(Frost_Demos)

# Fetch content of path file.
file(READ "../TargetPaths.txt" PATH_FILE_CONTENTS)

# Split the content by lines.
string(REGEX REPLACE "\r?\n" ";" PATH_FILE_LINES "${PATH_FILE_CONTENTS}")

# Fetch specific paths.
list(GET PATH_FILE_LINES 0 TARGET_LIB_PATH)
list(GET PATH_FILE_LINES 1 TARGET_INCLUDE_PATH)

set(TARGET_INCLUDE_PATH "${TARGET_INCLUDE_PATH}/Frost")

message(STATUS "Target Lib Path:" ${TARGET_LIB_PATH})
message(STATUS "Target Include Path:" ${TARGET_INCLUDE_PATH})

# Finds the Frost library on disk.
find_library(FROST_LIB NAMES Frost PATHS ${TARGET_LIB_PATH})

# Finds the required packages for Frost. nlohmann::json was found during the installation process
# since it is a purely header library.
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_executable(Demo demo.cpp)

set_target_properties(Demo PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

target_include_directories(Demo
PUBLIC
    ${TARGET_INCLUDE_PATH}
    ${TARGET_INCLUDE_PATH}/UITools
)

target_link_libraries(Demo
PRIVATE
    ${FROST_LIB}
    SDL2::SDL2
    SDL2_image
    SDL2_ttf
)


