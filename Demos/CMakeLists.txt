
# This sets the minimum cmake version, pretty straightforward.
cmake_minimum_required(VERSION 3.24)

# Name of your project goes here. This is more than simply a "name" for your project, Cmake uses
# this as a token to establish what it should include files for and link with. 
project(Frost_Demos)

# Path to the Frost Library on disk. This should be the same path as the one specified in the 
# "Frost_Path.txt" file.
set(FROST_PATH C:/Users/Joel/Frost)

# Finds the Frost library on disk, this line does not need to be modified.  
find_library(FROST_LIB NAMES Frost PATHS "${FROST_PATH}/lib")

# Finds the required packages for Frost. nlohmann::json was found during the installation process
# since it is a purely header library.
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

# Creates the executable from the project. The first parameter specifies the name of your 
# executable. The second is the name of your .cpp file that is being converted to an 
# executable. You will need to change the name of the .cpp file here to the name of your project's 
# .cpp file.
add_executable(Generic_Tester generic_tester.cpp)

# This line sets the executable's output directory to the same directory as this CMakeTextLists 
# file. Do note, if you change the path of this output directory, you will need to reinstall Frost
# with the updated "Project_Path.txt" file to your new executable directory. 
set_target_properties(Generic_Tester PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})


# This includes the include files from Frost for your project. Replace "Generic_Tester" with the 
# name of your executable. 
target_include_directories(Generic_Tester
PUBLIC
    ${FROST_PATH}/include
)

# Links the Frost library to the executable. Replace "Generic_Tester" with the 
# name of your executable.
target_link_libraries(Generic_Tester
PRIVATE
    ${FROST_LIB}
    SDL2::SDL2
    SDL2_image
)
