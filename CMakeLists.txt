cmake_minimum_required(VERSION 3.24)

project(Frost VERSION "0.1")

# Create static library
add_library(Frost STATIC ${SOURCES}
    
    src/ConsoleOutputHandler.cpp
    src/FileSystemHandler.cpp
    src/Fr_Math.cpp
    src/Fr_StringManip.cpp
    src/FrostEngine.cpp
    src/JsonHandler.cpp
    src/ProgramOutputHandler.cpp
    src/TextRenderingHandler.cpp
    src/TextureHandler.cpp
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
find_package(nlohmann_json REQUIRED)


# Link libraries

target_link_libraries(Frost
    PUBLIC
        SDL2::SDL2
        SDL2_image
)

# Get the target path for the user project from the external path file.

file(READ "Paths/Project_Path.txt" PROJECT_PATH)

# Copy the assets folder to the project

file(COPY "assets" DESTINATION ${PROJECT_PATH})

# Copy the data folder to the project

file(COPY "data" DESTINATION ${PROJECT_PATH})


# Fetch the target path for this static library from the external path file.

file(READ "Paths/Frost_Path.txt" FROST_PATH)

# Install the library.

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
    PATTERN "documentation" EXCLUDE
    PATTERN "assets" EXCLUDE
    PATTERN "Paths" EXCLUDE
    PATTERN "data" EXCLUDE
    PATTERN "*.exe" EXCLUDE
    PATTERN "*.txt" EXCLUDE
    PATTERN "*.cpp" EXCLUDE
    PATTERN "*.bat" EXCLUDE)
