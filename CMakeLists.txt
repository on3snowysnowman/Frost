cmake_minimum_required(VERSION 3.24)

project(Frost VERSION "0.2")


# Enable Degug Mode 
add_compile_definitions(FROST_DEBUG)

# Create static library
add_library(Frost STATIC 
    src/ConsoleOutputHandler.cpp
    src/FileSystemHandler.cpp
    src/Fr_Math.cpp
    src/Fr_Random.cpp
    src/Fr_StringManip.cpp
	src/Fr_StringManip.cpp
	src/FrostEngine.cpp
    src/InputHandler.cpp
    src/JsonHandler.cpp
    src/Menu.cpp
    src/MenuManager.cpp
    src/ProgramOutputHandler.cpp
    src/SpriteHandler.cpp
	src/TextFileHandler.cpp
    src/TextRenderingHandler.cpp
    src/TextureHandler.cpp
	src/TimeObserver.cpp
	src/UITools/UIChoice.cpp
	src/UITools/UIDisplayTools.cpp
	src/UITools/UIFloatVariable.cpp
	src/UITools/UIIntVariable.cpp
	src/UITools/UIItem.cpp
	src/UITools/UIList.cpp
	src/UITools/UISimulationTools.cpp
	src/UITools/UIStringVariable.cpp
	src/UITools/UIText.cpp
	src/UITools/UITextList.cpp
)

target_include_directories(Frost
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include/Frost
        ${CMAKE_SOURCE_DIR}/include/Frost/UITools
)

# Find required libraries 
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(nlohmann_json REQUIRED)

# Link Libraries
target_link_libraries(Frost
    PUBLIC
        SDL2::SDL2
        SDL2_image
)

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
install(TARGETS Frost ARCHIVE DESTINATION ${TARGET_LIB_PATH})
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/Frost DESTINATION ${TARGET_INCLUDE_PATH})
install(DIRECTORY ${CMAKE_SOURCE_DIR}/data DESTINATION ${TARGET_DATA_ASSETS_PATH})
install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${TARGET_DATA_ASSETS_PATH})

