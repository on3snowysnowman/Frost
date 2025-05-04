#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <SDL2/SDL.h>

#include "TextureHandler.hpp"
#include "DrawHandler.hpp"
#include "ConsoleOutputHandler.hpp"
#include "SpriteHandler.hpp"
#include "TextRenderingHandler.hpp"
#include "TimeObserver.hpp"
#include "Json.hpp"


/** Frost Engine by Joel Height. */
class FrostEngine
{

public:

    FrostEngine();
    ~FrostEngine();

    /** Starts the Engine. */
    void start();

    /** Gets the application's screen width in pixels. */
    static int get_screen_width();

    /** Gets the application's screen height in pixels. */
    static int get_screen_height();

    /** Gets the device's screen width in pixels. */
    static int get_native_screen_width();

    /** Gets the device's screen height in pixels.  */
    static int get_native_screen_height();

    /** Returns a readonly json object containing the data of the init file for the Engine. 
     * 
     * This method is available through the EventHandler using the "GET_INIT_DATA" invoke key.
    */
    const json& get_init_data() const;

    /**
     * @brief Overwrites the initialization data for the engine on the disk. Restart the program 
     * for changes to take place.
     * 
     * This method is available to invoke through the EventHandler using the "SAVE_INIT_DATA" 
     * invoke key.
     * 
     * @param new_init_data_json Data to save to disk. 
     */
    void save_new_init_data(const json& new_init_data_json);

protected:

    /** Called once per frame. Any user defined components that need to be updated can be placed.
     * here. */
    virtual void _user_update();

    /** Terminates Engine simulation. This function simply flags the Engine to stop on the next 
     * frame completion. Any change made between this function call and the end of the frame will
     * still happen. */
    void _quit();

    /** Sets the icon of the application window to a new png at the passed path. Returns true
     * if the png was successfully set.
     * 
     * @param path_to_png Path to the new png.
     */
    bool _set_application_icon(std::string path_to_png);
    
    // Returns a const reference to the internal variable measuring the time each frame takes.
    const double& _get_frame_time_reference();

    TextureHandler m_texture_handler;
    DrawHandler m_draw_handler;
    TextRenderingHandler m_text_ren_handler;
    ConsoleOutputHandler m_coh;
    SpriteHandler m_sprite_handler;

private:

    // Members 

    /**
     * If the Engine is active and should keep simulating. This bool is set to false when the quit
     * method is called. */
    bool m_is_active; 

    // Whether to use SDL's vsync functionality.
    bool m_use_vsync = false;

    double m_elapsed_milliseconds_this_frame; // Number of miliseconds this frame took.

    uint16_t m_target_fps {}; // Target frames per second that the Engine will simulate at.

    // Target miliseconds per frame to achieve target fps.
    float m_target_milliseconds_per_frame {};  

    // Timestamp of the beginning of the frame. Used to calculate the miliseconds each frame takes.
    c_time_point m_frame_start_timestamp; 

    static int s_screen_width; // Application screen width in pixels.
    static int s_screen_height; // Application screen height in pixels.
    static int s_native_screen_width; // Native screen width in pixels.
    static int s_native_screen_height; // Native screen height in pixels.

    // Path to the init data folder to setup the engine.
    const std::string m_INIT_DATA_DIRECTORY = "data/init"; 

    // Path to the base colors data file.
    const std::string m_BASE_COLOR_PATH = "data/init/base_colors.json";

    // Path to the extended colors data file.
    const std::string m_EXTENDED_COLOR_PATH = "data/init/extended_colors.json";

    json m_init_data_json; // Json object created from the contents of the data file. 

    SDL_Event m_event; // Instance of the SDL_Event.

    SDL_Surface* m_application_icon {}; // Icon for the application Window.

    SDL_Window* m_window;
    SDL_Renderer* m_renderer;


    // Methods

    /** Registers several Engine methods to the EventHandler. */
    void _register_events();

    /** Initializes SDL and creates SDL Components, along with the rest of the Engine. */
    void _init_SDL_and_engine();

    /** The core of the Engine. All processes are contained here, whether that be updating
     * components or rendering sprites to the screen. This update method is called each frame until
     * the Engine quits.
    */
    void _update();

    /** Loops the update() method with SDL vsync enabled, so no frame time delaying is 
     * needed. */
    void _simulation_loop_vsync();

    /** Loops the update() method while tracking frame times and delaying to ensure the 
     * FPS is throttled to the target FPS.
    */
    void _simulation_loop_no_vsync();

    /** Handles SDL_Events such as the application exit button being pressed, along with 
     * registering pressed keys on the keyboard. */
    void _handle_SDL_events();

    /** Flags the SDL_Renderer to clear itself. */
    void _clear_SDL_renderer();

    /** Flags the SDL_Renderer to present its buffered content. */
    void _present_SDL_renderer();
};
