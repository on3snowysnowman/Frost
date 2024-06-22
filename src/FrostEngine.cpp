#include <SDL2/SDL_image.h>

#include "FrostEngine.h"
#include "ProgramOutputHandler.h"
#include "FileSystemHandler.h"
#include "JsonHandler.h"


// Static Members

int FrostEngine::s_screen_width {};

int FrostEngine::s_screen_height {};


// Constructors / Deconstructor

FrostEngine::FrostEngine() : m_TARGET_MILISECONDS_PER_FRAME(1000 / m_TARGET_FPS) 
{
    // Clear the ProgramOutputHandler's output file
    ProgramOutputHandler::clear_output_file();

    // Initialize SDL and the Engine. 
    _init_SDL_and_engine();

    m_texture_handler = TextureHandler(m_renderer, m_color_data_path);

    m_text_ren_handler = TextRenderingHandler(&m_texture_handler);
    m_text_ren_handler.set_size_scale(2.0);

    m_coh = ConsoleOutputHandler(&m_texture_handler, 0, 0, s_screen_width, s_screen_height);

    m_is_active = true;

    // Begin simulation.
    _simulation_loop();
}

FrostEngine::~FrostEngine() 
{
    SDL_Quit();
}


// Public

int FrostEngine::get_screen_width() { return s_screen_width; }

int FrostEngine::get_screen_height() { return s_screen_height; }



// Protected

void FrostEngine::_quit() { m_is_active = false; }

bool FrostEngine::_set_application_icon(std::string path_to_png)
{
    // If the directory does not exist.
    if(!FileSystemHandler::does_directory_exist(path_to_png))
    {
        ProgramOutputHandler::log("FrostEnginer._set_application_icon() -> File: \""
            + path_to_png + "\" does not exist.", Frost::WARN);
        return false;
    }

    // If there is an existant icon, delete it so there is no memory leak.
    if(m_application_icon) delete m_application_icon;

    // Create the icon.
    m_application_icon = IMG_Load(path_to_png.c_str());

    SDL_SetWindowIcon(m_window, m_application_icon);

    return true;
}

// Private

void FrostEngine::_create_default_init_files_and_engine()
{
    // Create the data directory.
    FileSystemHandler::make_directory(m_init_data_directory);

    ProgramOutputHandler::log("Fullscreen: true");

    // Create the SDL Window
    m_window = SDL_CreateWindow("Frost", 0, 0, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);

    // Update the dimensions of the Engine to reflect the new dimensions of the fullscreen 
    // SDL_Window.
    SDL_GetWindowSize(m_window, &s_screen_width, &s_screen_height);

    // Create the SDL_Renderer.
    m_renderer = SDL_CreateRenderer(m_window, -1, 0);
    
    // Set the background color of the renderer.
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);

    // Stores temporary data to transfer to a json file.
    json temp;

    // Use temp to store default init data.

    temp["application_window_name"] = "Frost";

    temp["screen_width"] = s_screen_width;
    temp["screen_height"] = s_screen_height;

    temp["fullscreen"] = true;

    temp["background_color"] = json::array({20, 20, 30});

    // Dump the init data to the init file.
    JsonHandler::dump(temp, m_init_data_directory + "/init_data.json");

    temp.clear();

    // Use temp to store default color data.

    temp = json::array({});

    // Add the default color data to the temp json.
    temp.push_back(json::array({"Blue", 93, 97, 217}));
    temp.push_back(json::array({"Red", 201, 77, 77}));
    temp.push_back(json::array({"Green", 88, 184, 84}));
    temp.push_back(json::array({"Yellow", 222, 218, 111}));
    temp.push_back(json::array({"Orange", 224, 153, 90}));
    temp.push_back(json::array({"Purple", 141, 99, 219}));
    temp.push_back(json::array({"Brown", 153, 108, 84}));
    temp.push_back(json::array({"Dark_Gray", 153, 108, 84}));
    temp.push_back(json::array({"Light_Gray", 166, 167, 179}));
    temp.push_back(json::array({"White", 235, 235, 247}));

    // Dump the color data to the colors file.
    JsonHandler::dump(temp, m_init_data_directory + "/colors.json");

    // #TODO Place the color loading for the TextureHandler here.
}

void FrostEngine::_init_SDL_and_engine() 
{
    // If SDL failed to initialize.
    if(SDL_Init(SDL_INIT_EVENTS) != 0 || SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        ProgramOutputHandler::log("FrostEngine::_init_SDL() -> SDL failed to initialize.", 
            Frost::ERR);
        exit(1);
    }

    // Disable the cursor
    SDL_ShowCursor(SDL_DISABLE);

    // If the init folder does not exist in the working directory.
    if(!FileSystemHandler::does_directory_exist("data"))
    {
        ProgramOutputHandler::log("FrostEngine::_init_SDL_and_engine() -> \"data\" folder "
            "not found", Frost::ERR);
        exit(1);
    }

    // The data folder exists, assume the init files already exist.

    json init_data = JsonHandler::get(m_init_data_directory + "/init_data.json");

    std::string application_window_name = init_data.at("application_window_name");

    if(application_window_name.size() == 0) application_window_name = "Frost";

    if(init_data.at("fullscreen"))
    {
        ProgramOutputHandler::log("Fullscreen: true");

        // Create the SDL_Window as fullscreen.
        m_window = SDL_CreateWindow(application_window_name.c_str(), 0, 0, 0, 0, 
            SDL_WINDOW_FULLSCREEN_DESKTOP);

        SDL_GetWindowSize(m_window, &s_screen_width, &s_screen_height);
    }

    else
    {
        ProgramOutputHandler::log("Fullscreen: false");

        // Get the width and height from the data file.
        s_screen_width = init_data.at("screen_width");
        s_screen_height = init_data.at("screen_height");

        // Create the SDL_Window with the loaded data.
        m_window = SDL_CreateWindow(application_window_name.c_str(), SDL_WINDOWPOS_CENTERED, 
            SDL_WINDOWPOS_CENTERED, s_screen_width, s_screen_height, 0);
    }

    // Create the Renderer.
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);

    SDL_RenderSetVSync(m_renderer, 1);

    _set_application_icon("assets/Frost_Icon.png");
}

void FrostEngine::_simulation_loop()
{
    while(m_is_active)
    {
        m_frame_start_timestamp = SDL_GetTicks64();
        
        _handle_SDL_events();
        _clear_SDL_renderer();
        m_coh.render();
        _present_SDL_renderer();

        // Calculate the miliseconds this frame took.
        m_elapsed_miliseconds_this_frame = SDL_GetTicks64() - m_frame_start_timestamp;

        if(m_elapsed_miliseconds_this_frame < m_TARGET_MILISECONDS_PER_FRAME)
        {
            // Delay for the difference between the elapsed miliseconds and target miliseconds.
            SDL_Delay(m_TARGET_MILISECONDS_PER_FRAME - m_elapsed_miliseconds_this_frame);
        }
    }
}

void FrostEngine::_handle_SDL_events() 
{
    // While there are events.
    while(SDL_PollEvent(&m_event))
    {
        if(m_event.type == SDL_QUIT)
        {
            _quit();
        }
    }
}

void FrostEngine::_clear_SDL_renderer() { 
    SDL_RenderClear(m_renderer); 
}
  
void FrostEngine::_present_SDL_renderer() { SDL_RenderPresent(m_renderer); }
