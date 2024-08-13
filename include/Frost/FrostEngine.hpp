#pragma once

#include <cstdint>
#include <string>

#include <SDL2/SDL.h>

#include "TextureHandler.hpp"
#include "ConsoleOutputHandler.hpp"
#include "SpriteHandler.hpp"

#include "UIVariable.hpp"
#include "UIText.hpp"

class FrostEngine
{

public:

    FrostEngine();
    ~FrostEngine();

    /** Starts the Engine. */
    void start();

    /** Gets the screen width in pixels. */
    static int get_screen_width();

    /** Gets the screen height in pixels. */
    static int get_screen_height();


protected:

    /** Terminates Engine simulate. This function simply flags the Engine to stop on the next 
     * frame completion. Any change made between this function call and the end of the frame will
     * still happen. */
    void _quit();

    /** Sets the icon of the application window to a new png at the passed path. Returns true
     * if the png was sucessfuly set.
     * 
     * @param path_to_png Path to the new png.
     */
    bool _set_application_icon(std::string path_to_png);


private:

    // Members 

    /**
     * If the Engine is active and should keep simulating. This bool is set to false when the quit
     * method is called. */
    bool m_is_active; 

    // Whether to use SDL's vsync functionality.
    bool m_use_vsync = false;

    uint8_t m_elapsed_miliseconds_this_frame; // Number of miliseconds this frame took.

    uint8_t m_target_fps {}; // Target frames per second that the Engine will simulate at.

    // Target miliseconds per frame to achieve target fps.
    uint8_t m_target_miliseconds_per_frame {};  

    // Timestamp of the beginning of the frame. Used to calculate the miliseconds each frame takes.
    uint64_t m_frame_start_timestamp; 

    static int s_screen_width; // Screen width in pixels.
    static int s_screen_height; // Screen height in pixels.

    // Path to the init data folder to setup the engine.
    std::string m_init_data_directory = "data/init"; 

    // Path to the base colors data file.
    const std::string m_BASE_COLOR_PATH = "data/init/base_colors.json";

    // Path to the extended colors data file.
    const std::string m_EXTENDED_COLOR_PATH = "data/init/extended_colors.json";

    TextureHandler m_texture_handler;
    TextRenderingHandler m_text_ren_handler;
    ConsoleOutputHandler m_coh;
    SpriteHandler m_sprite_handler;

    SDL_Event m_event; // Instance of the SDL_Event.

    SDL_Surface* m_application_icon {}; // Icon for the application Window.

    SDL_Window* m_window;
    SDL_Renderer* m_renderer;

    // Methods

    /** Creates default init files, along with initializing the engine with default values. This 
     * method is called on Engine construction when no user created init files were found. */
    void _create_default_init_files_and_engine();

    /** Initializes SDL and creates SDL Components, along with the rest of the Engine. */
    void _init_SDL_and_engine();

    /** The core loop of the engine. This loop runs as long as the Engine is stil active (until 
     * the quit method is called). Each process of the engine is contained here, such as the 
     * updating of components and rendering methods. This simulation loop uses SDL's vsync 
     * functionality which means a frame limit does not need to be measured and enforced. */
    void _simulation_loop_vsync();

    /** The core loop of the engine. This loop runs as long as the Engine is stil active (until 
     * the quit method is called). Each process of the engine is contained here, such as the updating 
     * of components and rendering methods. This simulation loop does not use vsync, and therefore
     * frame times need to be monitored and altered to achieve target FPS. */
    void _simulation_loop_no_vsync();

    /** Handles SDL_Events such as the application exit button being pressed, along with 
     * registering pressed keys on the keyboard. */
    void _handle_SDL_events();

    /** Flags the SDL_Renderer to clear itself. */
    void _clear_SDL_renderer();

    /** Flags the SDL_Renderer to present its buffered content. */
    void _present_SDL_renderer();


    // DEMO

    UIItem::Status status = UIItem::HOVERED;

    std::string cursor_color = "Blue";

    UIText* text;
    UIVariable* variable;
};
