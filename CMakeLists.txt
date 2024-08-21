cmake_minimum_required(VERSION 3.24)

project(Frost VERSION "0.2")


# Enable Degug Mode 
add_compile_definitions(FROST_DEBUG)

# Collect all .cpp files in the source directory and subdirectories (recursive)
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create static library and link it with the sources.
add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include/Frost
        ${CMAKE_SOURCE_DIR}/include/Frost/UITools
)

# Find required libraries 
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(nlohmann_json REQUIRED)

# Link Libraries
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        SDL2::SDL2
        SDL2_image
)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

# Fetch content of path file
file(READ "TargetPaths.txt" PATH_FILE_CONTENTS)

# Split the content by lines
string(REGEX REPLACE "\r?\n" ";" PATH_FILE_LINES "${PATH_FILE_CONTENTS}")

# Assign each line to a variable
list(GET PATH_FILE_LINES 0 TARGET_LIB_PATH)
list(GET PATH_FILE_LINES 1 TARGET_INCLUDE_PATH)
list(GET PATH_FILE_LINES 2 TARGET_DATA_ASSETS_PATH)

message(STATUS "Target Lib Path:" ${TARGET_LIB_PATH})
message(STATUS "Target Include Path:" ${TARGET_INCLUDE_PATH})
message(STATUS "Target Data/Assets Path:" ${TARGET_DATA_ASSETS_PATH})

# Install each component to their target directory.
install(TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION ${TARGET_LIB_PATH})
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/Frost DESTINATION ${TARGET_INCLUDE_PATH})
install(DIRECTORY ${CMAKE_SOURCE_DIR}/data DESTINATION ${TARGET_DATA_ASSETS_PATH})
install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${TARGET_DATA_ASSETS_PATH})

