/**
 * @file FrostEngine.cpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Single class implementation.
 * @version 0.1
 * @date ?
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

#include "FrostEngine.hpp"
#include "FileSystemHandler.hpp"
#include "JsonHandler.hpp"
#include "InputHandler.hpp"
#include "MenuManager.hpp"
#include "EventHandler.hpp"
#include "ProgramOutputHandler.hpp"
#include "TextFileHandler.hpp"
#include "TimeObserver.hpp"
#include "CrashOutputHandler.hpp"


// Static Members

int FrostEngine::s_screen_width;

int FrostEngine::s_screen_height;

int FrostEngine::s_native_screen_width;

int FrostEngine::s_native_screen_height;

// Constructors / Deconstructor
#include "iostream"
FrostEngine::FrostEngine()
{ 
    #ifdef FROST_DEBUG

    // Clear the ProgramOutputHandler's output file
    ProgramOutputHandler::clear_output_file();
    ProgramOutputHandler::log("Program Started: " + TimeObserver::get_local_date() + " @ " + 
        TimeObserver::get_local_time() + '\n');
    ProgramOutputHandler::log("Debug Mode: true\n");
    #endif

    // Initialize SDL and the Engine. 
    _init_SDL_and_engine();

    _register_events();
}

FrostEngine::~FrostEngine() 
{
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    TTF_Quit();
    SDL_Quit();
}


// Public

void FrostEngine::start()
{
    m_is_active = true;

    // Begin Simulation

    if(m_use_vsync) _simulation_loop_vsync();

    else _simulation_loop_no_vsync();
}

int FrostEngine::get_screen_width() { return s_screen_width; }

int FrostEngine::get_screen_height() { return s_screen_height; }

int FrostEngine::get_native_screen_width() { return s_native_screen_width; }

int FrostEngine::get_native_screen_height() { return s_native_screen_height; }

const json& FrostEngine::get_init_data() const { return m_init_data_json; }

void FrostEngine::save_new_init_data(const json& new_init_data) 
{ 
    JsonHandler::dump(new_init_data, m_INIT_DATA_DIRECTORY + "/init_data.json"); 

    m_init_data_json = new_init_data;

    m_coh.set_font_path_and_size(new_init_data.at("font_size"), new_init_data.at("font_path"));
}


// Protected

void FrostEngine::_user_update() {}

void FrostEngine::_quit() 
{ 
    #ifdef FROST_DEBUG

    ProgramOutputHandler::log("Terminated Engine: " + TimeObserver::get_local_date() + " @ " + 
        TimeObserver::get_local_time() + '\n');
    #endif
    
    m_is_active = false; 
    
}

bool FrostEngine::_set_application_icon(std::string path_to_png)
{
    // If the directory does not exist.
    if(!FileSystemHandler::does_directory_exist(path_to_png))
    {
        #ifdef FROST_DEBUG

        ProgramOutputHandler::log("FrostEngine._set_application_icon() -> File: \""
            + path_to_png + "\" does not exist.", Frost::WARN);
        #endif

        return false;
    }

    // If there is an existant icon, delete it so there is no memory leak.
    if(m_application_icon) delete m_application_icon;

    // Create the icon.
    m_application_icon = IMG_Load(path_to_png.c_str());

    SDL_SetWindowIcon(m_window, m_application_icon);

    return true;
}

const double& FrostEngine::_get_frame_time_reference()
 { return m_elapsed_milliseconds_this_frame; }


// Private

void FrostEngine::_register_events()
{
    // Quits the simulation.
    EventHandler::register_event<void>(
        "QUIT_SIMULATION", 
        std::function<void()>(
            [this]() { this->_quit();}
    ));

    // Returns the init data for the engine.
    EventHandler::register_event<const json&>(
        "GET_INIT_DATA",
        std::function<const json&()>(
            [this]() -> const json& { return this->m_init_data_json; }
    ));

    // Saves new init data for the engine to disk.
    EventHandler::register_event<void, const json&>(
        "SET_INIT_DATA",
        std::function<void(const json&)>(
           [this](const json& new_init_data) { this->save_new_init_data(new_init_data); } 
    ));

    // Returns the native screen width.
    EventHandler::register_event<int>(
        "GET_NATIVE_SCREEN_WIDTH",
        std::function<int()>(
            [this]() { return get_native_screen_width(); }
    ));

    // Returns the native screen height.
    EventHandler::register_event<int>(
        "GET_NATIVE_SCREEN_HEIGHT",
        std::function<int()>(
            [this]() { return get_native_screen_height(); }
    ));
}

void FrostEngine::_init_SDL_and_engine() 
{
    // If SDL subsystems failed to initialize.
    if(SDL_Init(SDL_INIT_VIDEO))
    {
        OUTPUT_CRASH_DETAILS(" -> SDL failed to initialize: " + std::string(SDL_GetError()) + 
            ".\n");
        exit(1);
    }

    // IF SDL_ttf failed to initialize.
    if(TTF_Init())
    {
        OUTPUT_CRASH_DETAILS(" -> SDL_TTF failed to initialize: " + 
            std::string(SDL_GetError()));
        exit(1);
    }
    
    // Disable the cursor
    SDL_ShowCursor(SDL_DISABLE);

    // If the init folder does not exist in the working directory.
    if(!FileSystemHandler::does_directory_exist("data"))
    {
        OUTPUT_CRASH_DETAILS(" -> 'data' directory does not exist.\n");
        exit(1);
    }

    // The data folder exists, assume the init files already exist.

    m_init_data_json = JsonHandler::get(m_INIT_DATA_DIRECTORY + "/init_data.json");

    std::string application_window_name = m_init_data_json.at("application_window_name");

    if(application_window_name.size() == 0) application_window_name = "Frost";

    // Configure screen size and create window.

    // Get the display bounds of the primary monitor
    SDL_Rect fullscreen_dimensions;
    if (SDL_GetDisplayBounds(0, &fullscreen_dimensions) != 0) 
    {   
        OUTPUT_CRASH_DETAILS("SDL_GetDisplayBounds failed: " + std::string(SDL_GetError())
            + ".\n");
        exit(1);
    }

    s_native_screen_width = fullscreen_dimensions.w;
    s_native_screen_height = fullscreen_dimensions.h;


    if(m_init_data_json.at("fullscreen"))
    {
        #ifdef FROST_DEBUG

        ProgramOutputHandler::log("Fullscreen: true\n");
        #endif

        // Get the display bounds of the primary monitor
        // SDL_Rect fullscreen_dimensions;
        // if (SDL_GetDisplayBounds(0, &fullscreen_dimensions) != 0) 
        // {   
        //     OUTPUT_CRASH_DETAILS("SDL_GetDisplayBounds failed: " + std::string(SDL_GetError())
        //         + ".\n");
        //     exit(1);
        // }

        // Create a fullscreen window on the primary monitor
        m_window = SDL_CreateWindow(
            application_window_name.c_str(),
            fullscreen_dimensions.x, 
            fullscreen_dimensions.y,
            fullscreen_dimensions.w,
            fullscreen_dimensions.h,
            SDL_WINDOW_FULLSCREEN_DESKTOP
        );

        // Update the tracked screen width and height dimensions. 
        s_screen_width = fullscreen_dimensions.w;
        s_screen_height = fullscreen_dimensions.h;
    }

    else
    {
        #ifdef FROST_DEBUG

        ProgramOutputHandler::log("Fullscreen: false\n");
        #endif

        // Get the width and height from the data file.
        s_screen_width = m_init_data_json.at("screen_width");
        s_screen_height = m_init_data_json.at("screen_height");

        // Create the SDL_Window with the loaded data.
        m_window = SDL_CreateWindow(application_window_name.c_str(), SDL_WINDOWPOS_CENTERED, 
            SDL_WINDOWPOS_CENTERED, s_screen_width, s_screen_height, 0);
    }

    // Create the Renderer.
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);

    // Configure VSYNC.

    if(m_init_data_json.at("vsync"))
    {
        #ifdef FROST_DEBUG

        ProgramOutputHandler::log("Vsync: true\n");
        #endif

        m_use_vsync = true;

        // Enable Vsync.
        SDL_RenderSetVSync(m_renderer, 1);
    }

    else
    {
        #ifdef FROST_DEBUG

        ProgramOutputHandler::log("Vsync: false\n");
        #endif

        m_target_milliseconds_per_frame = 1000 / 
            static_cast<float>(m_init_data_json.at("frame_limit"));
    }

    _set_application_icon("assets/Frost_Icon.png");

    // Set background color.

    json background_color = m_init_data_json.at("background_color");

    SDL_SetRenderDrawColor(m_renderer, background_color.at(0), background_color.at(1), 
        background_color.at(2), 255);

    m_texture_handler = TextureHandler(m_renderer);

    // Configure colors and create RenderingHandler.

    if(m_init_data_json.at("use_extended_colors")) 
        m_draw_handler = DrawHandler(m_renderer, m_EXTENDED_COLOR_PATH,
            s_screen_width, s_screen_height);

    else m_draw_handler = DrawHandler(m_renderer, m_BASE_COLOR_PATH,
            s_screen_width, s_screen_height);

    // Create remaining components.
    
    m_text_ren_handler = TextRenderingHandler(&m_texture_handler, 
        &m_draw_handler, m_init_data_json.at("font_size"), 
        m_init_data_json.at("font_path"));
    
    m_coh = ConsoleOutputHandler(&m_texture_handler, &m_draw_handler,
        m_init_data_json.at("font_size"), m_init_data_json.at("font_path"), 
        0, 0, s_screen_width, s_screen_height);
    
    m_sprite_handler = SpriteHandler(&m_texture_handler, &m_draw_handler);

}

void FrostEngine::_update()
{
    // Establish timestamp of the start of the frame.
    m_frame_start_timestamp = TimeObserver::get_time_point();

    _clear_SDL_renderer();

    InputHandler::_reset_tracked_keys();

    _handle_SDL_events();

    if(InputHandler::is_key_pressed(SDLK_ESCAPE)) 
        EventHandler::invoke_event<void>("QUIT_SIMULATION");

    MenuManager::_update_active_menus();

    _user_update();

    m_coh._render();

    m_sprite_handler._render();

    _present_SDL_renderer();

    // Calculate the miliseconds this frame took.
    m_elapsed_milliseconds_this_frame = TimeObserver::calculate_interval_from_timepoints(
        m_frame_start_timestamp, TimeObserver::get_time_point());
}

void FrostEngine::_simulation_loop_vsync()
{
    while(m_is_active)
    {
        _update();

        // Since SDL vsync is enabled, no manual frame delay is needed.
        // If the measured frame time is less than the target frame time.
        if(m_elapsed_milliseconds_this_frame < m_target_milliseconds_per_frame)
        {
            // Delay for the difference between the measured miliseconds and target miliseconds.
            SDL_Delay(m_target_milliseconds_per_frame - m_elapsed_milliseconds_this_frame);

            // Override the milliseconds that were measure this frame, since an artificial delay 
            // has just been invoked to maintain target framerate.
            m_elapsed_milliseconds_this_frame = m_target_milliseconds_per_frame;
        }
    }
}

void FrostEngine:: _simulation_loop_no_vsync()
{
    while(m_is_active)
    {
        _update();

        // If the measured frame time is less than the target frame time.
        if(m_elapsed_milliseconds_this_frame < m_target_milliseconds_per_frame)
        {
            // Delay for the difference between the measured miliseconds and target miliseconds.
            SDL_Delay(m_target_milliseconds_per_frame - m_elapsed_milliseconds_this_frame);

            // Override the milliseconds that were measure this frame, since an artificial delay 
            // has just been invoked to maintain target framerate.
            m_elapsed_milliseconds_this_frame = m_target_milliseconds_per_frame;
        }
    }
}

void FrostEngine::_handle_SDL_events() 
{
    // While there are events.
    while(SDL_PollEvent(&m_event))
    {
        // Application exit button pressed.
        if(m_event.type == SDL_QUIT) { _quit(); }

        // Keyboard key was pressed.
        else if(m_event.type == SDL_KEYDOWN)
        { InputHandler::_flag_key_pressed(m_event.key.keysym.sym); }

        // Keyboard key was released.
        else if(m_event.type == SDL_KEYUP)
        { InputHandler::_flag_key_released(m_event.key.keysym.sym); }
    }
}

void FrostEngine::_clear_SDL_renderer() { SDL_RenderClear(m_renderer); }
  
void FrostEngine::_present_SDL_renderer() { SDL_RenderPresent(m_renderer); }
