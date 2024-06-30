# Frost ( Simple Text and 2D Image Engine ) by Joel Height #


# Description #

Frost is a lightweight 2D engine designed for rendering text and PNG images. It contains various 
tools for creating simple text menus along with support for sprite rendering. Uses 
SDL2 (Simple Direct Media Layer) as the foundation for rendering pngs on the screen, as well as 
handling events such as keyboard presses.


# Table of Contents #

    1: Uses & Setup
    2: Engine Functionality & Capabilities
    3: Getting Input
    4: Rendering Text
    5: Rendering Sprites
    6: Creating Menus
    7: UITools
    8: License
    9: Contact


# 1: Uses & Setup #

    USES:

        Frost can be used for a wide variety of applications, although it lacks the mass libraries 
    and functionality that a larger and more capable Engine may provide. Whether it be
    a simple text editor, menu interface system for storing and modifying data, or even a small 
    2D game with sprites, Frost provides the basics and foundations for creating such a project.
    
        Although 2D games can be created with Frost, this Engine does not do much of the heavy 
    lifting that comes with creating a project of that size. Frost is aimed toward smaller, more 
    basic text menus along with rendering pngs to the screen for informational and display
    purposes. In other words, Frost provides the framework for doing so, but much of the work will 
    need to be written by the user.  

    SETUP:

        Frost requires two dependencies to be installed on your pc, and available to the compiler. 
    These two dependencies are SDL2 (Simple Direct Media Library) and nlohmann::json. I recommend
    using a package manager like pacman in MSYS to install these packages. 

        The first step to the setting up Frost is to install it to your system. Frost installs as a
    statically linked library that your program will link to on compile. To specify where Frost 
    should install on your computer, navigate to the "Paths" directory. In here, you will find two
    files: 
    
        "Frost_Path.txt" is used to specify the path on your system where the Frost Library will be 
    placed. I typically put it under my users directory (C:/Users/Username).

        "Project_Path.txt" is used to specify the path to your project's executable directory that
    is going to be linking to this engine. This directory is important, because the required assets
    used by Frost will be copied into the same directory as the executable so that they are 
    available to it during runtime. 

        Once you've written both directories to their respective file, the next step is to run the 
    "install.bat" file. This file will use cmake to automatically build and install the project 
    into the specified directory and copy the assets into the directory of your project's 
    executable. After this step, the installation process of Frost is complete, and next is linking
    your project to Frost.

    LINKING:

        To link to Frost, use cmake. Inside the Demo directory, there are a collection of .cpp 
    files that demonstrate the basics of using Frost. Also contained, is the CMakeLists.txt file 
    that Cmake uses to build the project. If you examine that, you will find everything you need to
    build and link your project.

        


# 2: Engine Functionality & Capabilites #


# 3: Getting Input #


# 4: Rendering Text #


# 5: Rendering Sprites #


# 6: Creating Menus #


# 7: UITools #


# 8: License #

    Frost is released under the MIT License.

    Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
    and associated documentation files (the "Software"), to deal in the Software without 
    restriction, including without limitation the rights to use, copy, modify, merge, publish, 
    distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the 
    Software is furnished to do so, subject to the following conditions:

    ATTRIBUTION (Optional): While not required, users of this engine are encouraged to provide a 
    proper attribution notice in any distributed or derivative works. You may credit the engine by 
    including the following statement: "This engine is powered by Frost".

    MODIFICATIONS: Users who modify the engine must clearly mark those modifications in any 
    distributed versions of the engine, indicating what changes have been made.

    COMMERCIAL USE RESTRICTIONS: Commercial distribution of this engine as a standalone product is 
    prohibited. Users may only sell or distribute the engine commercially if substantial 
    modifications or value-added components have been incorporated.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
    BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND 
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
    DAMAGES, OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT, OR OTHERWISE, ARISING 
    FROM, OUT OF, OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

# 9: Contact #

    Name : Joel Height
    Email : On3SnowySnowman@gmail.com
