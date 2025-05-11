cmake_minimum_required(VERSION 3.24)

project(Frost_Demos)

# Fetch content of paths file.
file(READ "../TargetPaths.txt" PATH_FILE_CONTENTS)

# Split the content by lines.
string(REGEX REPLACE "\r?\n" ";" PATH_FILE_LINES "${PATH_FILE_CONTENTS}")

# Fetch specific paths.
list(GET PATH_FILE_LINES 0 TARGET_LIB_PATH)
list(GET PATH_FILE_LINES 1 TARGET_INCLUDE_PATH)

set(TARGET_INCLUDE_PATH "${TARGET_INCLUDE_PATH}")

message(STATUS "Target Lib Path:" ${TARGET_LIB_PATH})
message(STATUS "Target Include Path:" ${TARGET_INCLUDE_PATH})

# Finds the Frost library on disk.
find_library(FROST_LIB NAMES Frost PATHS ${TARGET_LIB_PATH})

# Finds the required packages for Frost. nlohmann::json was found during the installation process
# since it is a purely header library.
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Collect all .cpp files in demos_src
file(GLOB DEMO_SOURCES "${CMAKE_SOURCE_DIR}/demos_src/*.cpp")

foreach(DEMO_FILE ${DEMO_SOURCES})
    # Extract file name without extension to use as target name
    get_filename_component(DEMO_NAME ${DEMO_FILE} NAME_WE)

    # Define an executable for each demo file
    add_executable(${DEMO_NAME} ${DEMO_FILE})

    # Set the output directory for each demo binary
    set_target_properties(${DEMO_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

    # Add include directories
    target_include_directories(${DEMO_NAME}
    PUBLIC
        ${TARGET_INCLUDE_PATH}/Frost
        ${TARGET_INCLUDE_PATH}/Frost/UITools
        ${TARGET_INCLUDE_PATH}/nlohmann
        ${TARGET_INCLUDE_PATH}/FastNoiseLite

    )

    # Link libraries
    target_link_libraries(${DEMO_NAME}
    PRIVATE
        ${FROST_LIB}
        SDL2::SDL2
        SDL2_image
        SDL2_ttf
    )
endforeach()
