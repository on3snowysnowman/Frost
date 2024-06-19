#pragma once

#include <cstdint>

#include <SDL2/SDL.h>

class FrostEngine
{

public:

    FrostEngine();
    ~FrostEngine();

    /** Gets the screen width in pixels. */
    static int get_screen_width();

    /** Gets the screen height in pixels. */
    static int get_screen_height();

protected:

    /** Terminates Engine simulate. This function simply flags the Engine to stop on the next 
     * frame completion. Any change made between this function call and the end of the frame will
     * still happen. */
    void quit();


private:

    // Members 

    /**
     * If the Engine is active and should keep simulating. This bool is set to false when the quit
     * method is called. */
    bool m_is_active; 

    uint8_t m_elapsed_miliseconds_this_frame; // Number of miliseconds this frame took.

    // Target miliseconds per frame to achieve target fps.
    const uint8_t TARGET_MILISECONDS_PER_FRAME;  

    const uint16_t TARGET_FPS = 60; // Target frames per second that the Engine will simulate at.

    // Timestamp of the beginning of the frame. Used to calculate the miliseconds each frame takes.
    uint64_t m_frame_start_timestamp; 

    static int s_screen_width; // Screen width in pixels.
    static int s_screen_height; // Screen height in pixels.

    SDL_Event event; // Instance of the SDL_Event.

    SDL_Window* m_window; // Instance of the SDL_Window.
    SDL_Renderer* m_renderer; // Instance of the SDL_Renderer.


    // Methods

    /** Initializes SDL and creates SDL Components. */
    void _init_SDL();

    /** Begins engine simulation. */
    void _start_simulation();

    /** The core loop of the engine. This loop runs as long as the Engine is stil active (until 
     * the quit method is called). Each process of the engine is contained here, such as the updating 
     * of components and rendering methods. */
    void _simulation_loop();

    /** Handles SDL_Events such as the application exit button being pressed, along with 
     * registering pressed keys on the keyboard. */
    void _handle_SDL_events();

    /** Flags the SDL_Renderer to clear itself */
    void _clear_SDL_renderer();

    /** Flags the SDL_Renderer to present its buffered content */
    void _present_SDL_renderer();
};
