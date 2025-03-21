# Frost ( Simple Text and 2D Image Engine ) by Joel Height #

# NOTE #
    
    For any confusion why the assets/data folders are copied into my Demos folder, along with
    the Demos linking to the Engine on disk instead of inside the project: The Demos folder is 
    meant to simulate an independent project in another directory, that needs the respective data 
    and assets files during runtime, along with being able to link to the engine. Having the Demos 
    structured in this way, where the Engine is first built and copied into disk, ensures the final
    product will work the way I intend it to, rather than simply fetching and linking files from 
    inside the project, files which a projects using this Engine won't have. 

# Description #

    Frost is a lightweight 2D engine designed for rendering text and PNG images. It contains various 
    tools for creating simple text menus along with support for sprite rendering. Uses 
    SDL2 (Simple Direct Media Layer) as the foundation for rendering pngs on the screen, as well as 
    handling events such as keyboard presses.

    This is a passion project of mine to further learn and develop my skills as a programmer, along
    with challenging myself to see how optimized I can make my code. Inside the project, you will see
    quite redundant "optimizations" such as using uint8_ts instead of ints and things like that. 
    I am well aware that many of these micro optimizations are mostly negligable when it comes to 
    performance, along with sometimes being dangerous as spending too much time micro optimizing is
    a well known problem. The main optimizations come with crafting the right algorithms and using 
    the cache in an efficient way. Since this project has no "deadline" and is not meant to be a 
    project that fills any larger-world role, I am content with spending more than normal time 
    improving code that would run feasibly in the larger picture. The goal of this project is to 
    stress my skills and find any way to optimize, all for the learning process.

    This README will most likely be changed quite frequently in the future as the engine changes
    and progesses. 


# Table of Contents #

    Uses
    Setup
    Engine Functionality & Capabilities
    Getting Input
    Rendering Text
    Rendering Sprites
    Creating Menus
    UITools
    License
    Contact


# Uses #


    Frost is versatile and can be used for a range of applications, including:  
    - Simple text editors  
    - Menu interface systems for storing and modifying data  
    - Small 2D games with sprites  

    However, it should be noted that Frost lacks the extensive libraries and functionality of 
    more robust engines. It is more suitable for smaller-scale projects that focus on text 
    rendering and basic sprite handling. While Frost provides the foundational elements, users 
    will need to implement additional features for a complete project.

    The engine is distributed as a static library, and projects can link to it during 
    compile-time.

# Setup #

    To set up the engine, it must first be built and installed on your system. The recommended 
    build system is CMake. To specify installation paths for the static library, include files, 
    and runtime assets, use the `TargetPaths.txt` file located in the main directory. The first 
    line specifies the static library installation path, the second line specifies the include 
    file path, and the third line points to where runtime dependencies (i.e., `/assets` and 
    `/data`) should be placed. Typically, these runtime dependencies should reside in the same 
    directory as the executable being built.


# Engine Functionality & Capabilites #

    The main Engine runs in a simulation loop that updates each component each frame. To use the 
    Engine and this update loop, simply create a custom user-defined Engine class and inherit the 
    FrostEngine class. No constructor parameters are needed as the configuration


# Getting Input #


# Rendering Text #


# Rendering Sprites #


# Creating Menus #


# UITools #


# License #

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

# Contact #

    Name : Joel Height
    Email : On3SnowySnowman@gmail.com
