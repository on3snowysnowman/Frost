cmake_minimum_required(VERSION 3.24)

project(Frost VERSION "0.1")

# Create static library
add_library(Frost STATIC ${SOURCES}
    src/FrostEngine.cpp
)

target_include_directories(Frost
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

# Find required libraries

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)


# Link libraries

target_link_libraries(Frost
    PUBLIC
        SDL2::SDL2
        SDL2_image
)

# Fetch the target path for this static library from the external path file

file(STRINGS "Frost_Path.txt" FROST_PATH)

# Install the library 

install(TARGETS Frost ARCHIVE DESTINATION "${FROST_PATH}/Frost/lib")

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION "${FROST_PATH}/Frost/"
    PATTERN "build" EXCLUDE
    PATTERN "src" EXCLUDE
    PATTERN ".vscode" EXCLUDE
    PATTERN ".git" EXCLUDE
    PATTERN ".gitignore" EXCLUDE
    PATTERN ".vscode" EXCLUDE
    PATTERN "old_files" EXCLUDE
    PATTERN "Demos" EXCLUDE
    PATTERN "*.exe" EXCLUDE
    PATTERN "*.txt" EXCLUDE
    PATTERN "*.cpp" EXCLUDE
    PATTERN "*.bat" EXCLUDE)
