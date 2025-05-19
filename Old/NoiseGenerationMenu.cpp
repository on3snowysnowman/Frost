
#include <iostream>
#include <string>

#include "FrostEngine.hpp"
#include "InputHandler.hpp"
#include "ConsoleOutputHandler.hpp"
#include "DrawHandler.hpp"
#include "Menu.hpp"
#include "MenuManager.hpp"
#include "UISimulationContainers.hpp"
#include "FastNoiseLite.h"
#include "UIIntVariable.hpp"
#include "UIFloatVariable.hpp"
#include "UIList.hpp"
#include "UIButton.hpp"
#include "CrashOutputHandler.hpp"
#include "DrawHandler.hpp"
#include "UIButton.hpp"
#include "UISimulationTools.hpp"
#include "Fr_Random.hpp"
#include "Fr_IntLimits.hpp"
#include "NoiseGenerator.hpp"


class NoiseViewMenu : public Menu
{

public:

    NoiseViewMenu() : Menu("NOISE_VIEW") 
    {
        m_coh = nullptr;
        m_renderer = nullptr;
        m_draw_handler = nullptr;
    }

    NoiseViewMenu(ConsoleOutputHandler* coh, SDL_Renderer* renderer,
        DrawHandler* draw_handler) 
    {
        m_coh = coh;
        m_renderer = renderer;
        m_draw_handler = draw_handler;

        m_noise_gen.get_noise_generator().SetFractalType(
            FastNoiseLite::FractalType_FBm);

        m_noise_seed_UI = UIIntVariable(m_coh, &m_cursor_color, "Seed");
        m_noise_width_UI = UIIntVariable(m_coh, &m_cursor_color, "Width");
        m_noise_height_UI = UIIntVariable(m_coh, &m_cursor_color, "Height");
        // m_condensed_width_UI = UIIntVariable(m_coh, &m_cursor_color, 
        //     "Condensed Width");
        // m_condensed_height_UI = UIIntVariable(m_coh, &m_cursor_color, 
        //     "Condensed Height");
        m_zoom_factor_UI = UIIntVariable(m_coh, &m_cursor_color, "Zoom Factor");
        // m_noise_rendering_scale_UI = UIIntVariable(m_coh, &m_cursor_color, 
        //     "Noise Render Scale", "1");
        m_noise_frequency_UI = UIFloatVariable(m_coh, &m_cursor_color, 
            "Frequency");
        m_noise_octaves_UI = UIIntVariable(m_coh, &m_cursor_color, "Octaves");
        m_noise_lacunarity_UI = UIFloatVariable(m_coh, &m_cursor_color, 
            "Lacunarity");
        m_noise_gain_UI = UIFloatVariable(m_coh, &m_cursor_color, 
            "Gain");

        m_new_seed_button_UI = UIButton<NoiseViewMenu>(m_coh, &m_cursor_color, 
            "Get New Seed", "Purple", this, &NoiseViewMenu::_get_new_seed);

        m_generate_noise_button_UI = UIButton<NoiseViewMenu>(m_coh, 
            &m_cursor_color, "Generate Noise", "Green", this, 
            &NoiseViewMenu::_generate_new_noise);

        m_col_attribs_UI = UIList(m_coh, &m_cursor_color, 
            "Color Range Attributes", {});

        m_add_col_attrib_button_UI = UIButton<NoiseViewMenu>(m_coh, 
            &m_cursor_color, "Add Color Attribute", "Green", this,
            &NoiseViewMenu::_add_empty_col_attrib);

        m_del_col_attrib_button_UI = UIButton<NoiseViewMenu>(m_coh, 
            &m_cursor_color, "Delete Color Attribute", "Red", this,
            &NoiseViewMenu::_del_col_attrib);
        
        _add_empty_col_attrib();
        m_col_attribs_UI.m_content.push_back(&m_add_col_attrib_button_UI);
        m_col_attribs_UI.m_content.push_back(&m_del_col_attrib_button_UI);

        m_panel = UIPanelContainer(m_coh, &m_cursor_color);

        m_panel.content.push_back(&m_noise_seed_UI);

        m_panel.content.push_back(&m_noise_width_UI);
        m_panel.content.push_back(&m_noise_height_UI);
        // m_panel.content.push_back(&m_condensed_width_UI);
        // m_panel.content.push_back(&m_condensed_height_UI);
        m_panel.content.push_back(&m_zoom_factor_UI);
        m_panel.content.push_back(&m_noise_frequency_UI);
        m_panel.content.push_back(&m_noise_octaves_UI);
        m_panel.content.push_back(&m_noise_lacunarity_UI);
        m_panel.content.push_back(&m_noise_gain_UI);
        m_panel.content.push_back(&m_col_attribs_UI);
        m_panel.content.push_back(&m_new_seed_button_UI);
        m_panel.content.push_back(&m_generate_noise_button_UI);

        m_noise_src_dimensions.x = 0;
        m_noise_src_dimensions.y = 0;
        m_noise_dest_dimensions.x = 10;
        m_noise_dest_dimensions.y = 10;

        m_condensed_src_dimensions.x = 0;
        m_condensed_src_dimensions.y = 0;
    }

    void start() final 
    { 
        m_noise_seed_UI.content = "0";

        m_noise_width_UI.content = "100";
        m_noise_height_UI.content = "100";

        // m_condensed_width_UI.content = "10";
        // m_condensed_height_UI.content = "10";

        // m_noise_rendering_scale_UI.content = "14";

        m_noise_frequency_UI.content = "0.03";

        m_noise_octaves_UI.content = "3";
        m_noise_lacunarity_UI.content = "2.0";
        m_noise_gain_UI.content = "0.5";

        m_panel.reset();

        _generate_new_noise(); 
    }

    void update() final 
    {
        float frequency;

        bool generate_new_noise = false;
        
        for(const Key key : InputHandler::get_pressed_and_available_keys())
        {
            switch(key)
            {
                case SDLK_SPACE:    

                    // _generate_new_noise();
                    generate_new_noise = true;
                    InputHandler::block_key_until_released(SDLK_SPACE);
                    break;

                case SDLK_n:    

                    _get_new_seed();
                    InputHandler::block_key_until_released(SDLK_n);
                    break;

                case SDLK_UP:   

                    m_y_offset -= 1;
                    // _generate_new_noise();
                    generate_new_noise = true;
                    break;
                
                case SDLK_RIGHT:    

                    m_x_offset += 1;
                    // _generate_new_noise();
                    generate_new_noise = true;
                    break;
                
                case SDLK_DOWN: 

                    m_y_offset += 1;
                    // _generate_new_noise();
                    generate_new_noise = true;
                    break;

                case SDLK_LEFT:

                    m_x_offset -= 1;
                    // _generate_new_noise();
                    generate_new_noise = true;
                    break;

                case SDLK_LEFTBRACKET:  

                    frequency = std::stof(m_noise_frequency_UI.content);
                    frequency += 0.001;
                    m_noise_frequency_UI.content = std::to_string(frequency);
                    InputHandler::delay_key(SDLK_LEFTBRACKET);
                    // _generate_new_noise();
                    generate_new_noise = true;
                    break;

                case SDLK_RIGHTBRACKET:  

                frequency = std::stof(m_noise_frequency_UI.content);
                frequency -= 0.001 * (frequency > 0.001);
                m_noise_frequency_UI.content = std::to_string(frequency);
                InputHandler::delay_key(SDLK_RIGHTBRACKET);
                // _generate_new_noise();
                generate_new_noise = true;
                break;
            }
        }

        if(generate_new_noise) _generate_new_noise();

        m_draw_handler->draw(m_noise_texture, m_noise_src_dimensions, 
            m_noise_dest_dimensions);
        m_draw_handler->draw(m_condensed_noise_texture, 
            m_condensed_src_dimensions, m_condensed_dest_dimensions);
        m_draw_handler->draw(m_colored_noise_texture,
            m_noise_src_dimensions, m_colored_dest_dimensions);
        m_draw_handler->draw(m_col_cond_noise_texture,
            m_condensed_src_dimensions, m_col_cond_dest_dimensions);

        m_coh->set_anchor(60);

        Frost::simulate_UI_panel(m_panel);
    }


private:

    // Members

    uint16_t m_noise_width {};
    uint16_t m_noise_height {};

    uint16_t m_condensed_width {};
    uint16_t m_condensed_height {};

    int m_x_offset {};
    int m_y_offset {};

    SDL_Rect m_noise_src_dimensions;
    SDL_Rect m_noise_dest_dimensions;
    SDL_Rect m_condensed_src_dimensions;
    SDL_Rect m_condensed_dest_dimensions;
    SDL_Rect m_colored_dest_dimensions;
    SDL_Rect m_col_cond_dest_dimensions;

    std::string m_cursor_color = "Blue";

    std::vector<std::pair<float, SDL_Color>> m_color_attribs;

    UIIntVariable m_noise_seed_UI;

    UIIntVariable m_noise_width_UI;
    UIIntVariable m_noise_height_UI;
    UIIntVariable m_zoom_factor_UI;
    // UIIntVariable m_condensed_width_UI;
    // UIIntVariable m_condensed_height_UI;
    // UIIntVariable m_noise_rendering_scale_UI;
    
    UIFloatVariable m_noise_frequency_UI;

    UIIntVariable m_noise_octaves_UI;
    UIFloatVariable m_noise_lacunarity_UI;
    UIFloatVariable m_noise_gain_UI;

    UIList m_col_attribs_UI;

    UIButton<NoiseViewMenu> m_add_col_attrib_button_UI;
    UIButton<NoiseViewMenu> m_del_col_attrib_button_UI;

    UIButton<NoiseViewMenu> m_new_seed_button_UI;
    UIButton<NoiseViewMenu> m_generate_noise_button_UI;

    UIPanelContainer m_panel;

    ConsoleOutputHandler* m_coh;

    // FastNoiseLite m_noise_gen;

    NoiseGenerator m_noise_gen;

    SDL_Texture* m_noise_texture {};

    SDL_Texture* m_condensed_noise_texture {};

    SDL_Texture* m_colored_noise_texture {};

    SDL_Texture* m_col_cond_noise_texture {};

    SDL_Renderer* m_renderer;

    DrawHandler* m_draw_handler;


    // Methods

    void _del_col_attrib()
    {
        if(m_col_attribs_UI.m_content.size() == 2) return;

        delete m_col_attribs_UI.m_content.front();

        m_col_attribs_UI.m_content.erase(m_col_attribs_UI.m_content.begin());

        --m_col_attribs_UI.m_cursor_index;
    }

    void _add_empty_col_attrib()
    {
        UIList* color_selection_for_new_attribute = 
            new UIList(m_coh, &m_cursor_color, "Color", {});
        
        color_selection_for_new_attribute->m_content.push_back(
            new UIFloatVariable(m_coh, &m_cursor_color, "Noise constraint"));
        color_selection_for_new_attribute->m_content.push_back(
            new UIIntVariable(m_coh, &m_cursor_color, "r"));
        color_selection_for_new_attribute->m_content.push_back(
            new UIIntVariable(m_coh, &m_cursor_color, "g"));
        color_selection_for_new_attribute->m_content.push_back(
            new UIIntVariable(m_coh, &m_cursor_color, "b"));    

        m_col_attribs_UI.m_content.insert(m_col_attribs_UI.m_content.begin(), 
            color_selection_for_new_attribute);

        ++m_col_attribs_UI.m_cursor_index;
    }

    /**
     * @brief Gets a new seed from the current time and seeds the Noise 
     * Generator.
     */
    void _get_new_seed()
    {
        // Get a new seed from the current time.
        int new_seed = FrostRandom::get_random_num<int>(0, Frost::INT32_LIMIT);

        m_noise_seed_UI.content = std::to_string(new_seed);
    }

    /**
     * @brief Configures the settings of the Noise Generator using the values
     * from the UI elements.uint32_t
     */
    void _configure_noise_gen()
    {
        // Fetch config values from the UI content.
        int seed = std::stoi(m_noise_seed_UI.content);

        m_noise_width = std::stoi(m_noise_width_UI.content);
        m_noise_height = std::stoi(m_noise_height_UI.content);

        // m_condensed_width = std::stoi(m_condensed_width_UI.content);
        // m_condensed_height = std::stoi(m_condensed_height_UI.content);

        m_condensed_width = m_noise_width;
        m_condensed_height = m_noise_height;

        // int noise_rendering_scale = std::stoi(
        //     m_noise_rendering_scale_UI.content);

        int noise_rendering_scale = 300 / m_noise_width;

        int condensed_rendering_scale = (m_noise_width / m_condensed_width)
            * noise_rendering_scale;

        m_noise_src_dimensions.w = m_noise_width;
        m_noise_src_dimensions.h = m_noise_height;

        m_noise_dest_dimensions.w = m_noise_width * noise_rendering_scale;
        m_noise_dest_dimensions.h = m_noise_height * noise_rendering_scale;

        m_condensed_src_dimensions.w = m_condensed_width;
        m_condensed_src_dimensions.h = m_condensed_height;

        m_condensed_dest_dimensions.x = 10;
        m_condensed_dest_dimensions.y = 
            m_noise_dest_dimensions.y + m_noise_dest_dimensions.h + 10;
        m_condensed_dest_dimensions.w = m_condensed_width * 
            condensed_rendering_scale;
        m_condensed_dest_dimensions.h = m_condensed_height * 
            condensed_rendering_scale;

        m_colored_dest_dimensions.x = m_noise_dest_dimensions.x + 
            m_noise_dest_dimensions.w + 10;
        m_colored_dest_dimensions.y = 10;
        m_colored_dest_dimensions.w = m_noise_dest_dimensions.w;
        m_colored_dest_dimensions.h = m_noise_dest_dimensions.h;

        m_col_cond_dest_dimensions.x = m_noise_dest_dimensions.x + 
            m_noise_dest_dimensions.w + 10;
        m_col_cond_dest_dimensions.y = m_noise_dest_dimensions.y + 
            m_noise_dest_dimensions.h + 10;
        m_col_cond_dest_dimensions.w = m_condensed_dest_dimensions.w;
        m_col_cond_dest_dimensions.h = m_condensed_dest_dimensions.h;

        m_noise_gen.set_offset(m_x_offset, m_y_offset);

        FastNoiseLite& underlying_generator = 
            m_noise_gen.get_noise_generator();
        
        underlying_generator.SetSeed(seed);
        underlying_generator.SetFrequency(std::stof(
            m_noise_frequency_UI.content));
        underlying_generator.SetFractalOctaves(std::stoi(
            m_noise_octaves_UI.content));
        underlying_generator.SetFractalLacunarity(std::stof(
            m_noise_lacunarity_UI.content));
        underlying_generator.SetFractalGain(std::stof(
            m_noise_gain_UI.content));
    }

    void _generate_new_noise() 
    {
        _configure_noise_gen();

        std::vector<float> noise_map = 
            m_noise_gen.generate_full_noise_map(m_noise_width, m_noise_height);

        _generate_noise_texture(noise_map);

        // std::vector<float> condensed_noise = 
        //     m_noise_gen.condense_noise_map(noise_map, m_noise_width, 
        //     m_noise_height, m_condensed_width, m_condensed_height);

        int zoom_scale_factor = std::stoi(m_zoom_factor_UI.content);

        std::vector<float> zoomed_out_noise_map = 
            m_noise_gen.generate_full_noise_map(m_noise_width * 
            zoom_scale_factor, m_noise_height * zoom_scale_factor);
        
        std::vector<float> condensed_noise_map = 
            m_noise_gen.condense_noise_map(zoomed_out_noise_map, 
            m_noise_width * zoom_scale_factor, m_noise_height * 
            zoom_scale_factor, m_noise_width, m_noise_height);

        _generate_condensed_noise_texture(condensed_noise_map);

        _fetch_color_attribs();

        // std::vector<SDL_Color> colored_noise =
        //     m_noise_gen.colorize_noise_map(noise_map, 
        //     {
        //         {.3, SDL_Color {0, 0, 200}}, 
        //         {1, SDL_Color {0, 200, 0}}
        //         // {1, SDL_Color {120, 120, 120}}
        //     });

        std::vector<SDL_Color> colored_noise_map = 
            m_noise_gen.colorize_noise_map(noise_map, m_color_attribs);
        
        _generate_colored_noise_texture(colored_noise_map);

        std::vector<SDL_Color> col_cond_noise_map = 
            m_noise_gen.colorize_noise_map(condensed_noise_map, m_color_attribs);

        _generate_col_cond_noise_texture(col_cond_noise_map);
    }

    void _fetch_color_attribs() 
    {
        m_color_attribs.clear();

        std::pair<float, SDL_Color> fetched_attrib;

        for(int i = 0; i < m_col_attribs_UI.m_content.size() - 2; ++i)
        {
            UIList* targ_attrib_in_UI = static_cast<UIList*>(
                m_col_attribs_UI.m_content.at(i));
            
            fetched_attrib.first = std::stof(static_cast<UIFloatVariable*>(
                targ_attrib_in_UI->m_content.at(0))->content);

            fetched_attrib.second.r = std::stoi(static_cast<UIIntVariable*>(
                targ_attrib_in_UI->m_content.at(1))->content);
            fetched_attrib.second.g = std::stoi(static_cast<UIIntVariable*>(
                targ_attrib_in_UI->m_content.at(2))->content);
            fetched_attrib.second.b = std::stoi(static_cast<UIIntVariable*>(
                targ_attrib_in_UI->m_content.at(3))->content);

            m_color_attribs.push_back(fetched_attrib);
        }
    }

    void _generate_noise_texture(const std::vector<float>& noise_map)
    {
        if(m_noise_texture) SDL_DestroyTexture(m_noise_texture);

        SDL_Surface* noise_surface = SDL_CreateRGBSurfaceWithFormat(0, 
            m_noise_width, m_noise_height, 32, SDL_PIXELFORMAT_RGBA32);

        // Get a pointer to the main noise Surface's pixel array.
        uint32_t* surface_pixels = static_cast<uint32_t*>(noise_surface->pixels);

        bool surface_is_locked = false;

        // This Surface must be locked for pixel access.
        if(SDL_MUSTLOCK(noise_surface))
        {
            surface_is_locked = true;
            SDL_LockSurface(noise_surface);
        }

        // Iterate through each pixel in the surface and generated a noise value
        // for it.
        for(int row = 0; row < m_noise_height; ++row)
        {
            for(int col = 0; col < m_noise_width; ++col)
            {
                // Get the value off the grayscale color at this pixel position.
                // Since the generator gives values from -1 to 1, map the noise 
                // value from [-1, 1] to [0, 255].
                uint32_t pixel_grayscale_value = (noise_map.at(
                    (row * m_noise_width) + col) + 1) * 122;

                // Set the pixel value of the Surface at this location.
                surface_pixels[(row * m_noise_width) + col] = 
                    SDL_MapRGBA(noise_surface->format, pixel_grayscale_value, 
                        pixel_grayscale_value, pixel_grayscale_value, 255);
            }
        }

        if(surface_is_locked)
        {
            SDL_UnlockSurface(noise_surface);
            surface_is_locked = false;
        }

        m_noise_texture = SDL_CreateTextureFromSurface(m_renderer, 
            noise_surface);

        SDL_FreeSurface(noise_surface);
    }

    void _generate_condensed_noise_texture(const std::vector<float>& noise_map) 
    {
        if(m_condensed_noise_texture) 
            SDL_DestroyTexture(m_condensed_noise_texture);

        SDL_Surface* noise_surface = SDL_CreateRGBSurfaceWithFormat(
            0, m_condensed_width, m_condensed_height, 32, 
            SDL_PIXELFORMAT_RGBA32);

        uint32_t* surface_pixels = static_cast<uint32_t*>(
            noise_surface->pixels);

        bool surface_is_locked = false;

        if(SDL_MUSTLOCK(noise_surface))
        {
            surface_is_locked = true;
            SDL_LockSurface(noise_surface);
        }

        for(int row = 0; row < m_condensed_height; ++row)
        {
            for(int col = 0; col < m_condensed_width; ++col)
            {
                // Get the value off the grayscale color at this pixel position.
                // Since the generator gives values from -1 to 1, map the noise 
                // value from [-1, 1] to [0, 255].
                uint32_t pixel_grayscale_value = (noise_map.at(
                    (row * m_condensed_width) + col) + 1) * 122;

                // Set the pixel value of the Surface at this location.
                surface_pixels[(row * m_condensed_width) + col] = 
                    SDL_MapRGBA(noise_surface->format, pixel_grayscale_value, 
                        pixel_grayscale_value, pixel_grayscale_value, 255);
            }
        }

        if(surface_is_locked)
        {
            SDL_UnlockSurface(noise_surface);
        }

        m_condensed_noise_texture = SDL_CreateTextureFromSurface(m_renderer,
            noise_surface);

        SDL_FreeSurface(noise_surface);
    }

    void _generate_colored_noise_texture(const std::vector<SDL_Color>& noise_map) 
    {
        if(m_colored_noise_texture) SDL_DestroyTexture(m_colored_noise_texture);
        
        SDL_Surface* noise_surface = SDL_CreateRGBSurfaceWithFormat(
            0, m_noise_width, m_noise_height, 32, SDL_PIXELFORMAT_RGBA32);

        uint32_t* surface_pixels = static_cast<uint32_t*>(
            noise_surface->pixels);

        bool surface_is_locked = false;

        if(SDL_MUSTLOCK(noise_surface))
        {
            surface_is_locked = true;
            SDL_LockSurface(noise_surface);
        }

        for(int row = 0; row < m_noise_height; ++row)
        {
            for(int col = 0; col < m_noise_width; ++col)
            {
                const SDL_Color& color_value = 
                    noise_map.at((row * m_noise_width) + col);

                // Set the pixel value of the Surface at this location.
                surface_pixels[(row * m_noise_width) + col] = 
                    SDL_MapRGBA(noise_surface->format, color_value.r, 
                        color_value.g, color_value.b, 255);
            }
        }

        if(surface_is_locked)
        {
            SDL_UnlockSurface(noise_surface);
        }

        m_colored_noise_texture = SDL_CreateTextureFromSurface(m_renderer,
            noise_surface);

        SDL_FreeSurface(noise_surface);
    }

    void _generate_col_cond_noise_texture(const std::vector<SDL_Color>& noise_map)
    {
        if(m_col_cond_noise_texture) 
            SDL_DestroyTexture(m_col_cond_noise_texture);

        SDL_Surface* noise_surface = SDL_CreateRGBSurfaceWithFormat(
            0, m_condensed_width, m_condensed_height, 32, SDL_PIXELFORMAT_RGBA32);

        uint32_t* surface_pixels = static_cast<uint32_t*>(
            noise_surface->pixels);

        bool surface_is_locked = false;

        if(SDL_MUSTLOCK(noise_surface))
        {
            surface_is_locked = true;
            SDL_LockSurface(noise_surface);
        }

        for(int row = 0; row < m_condensed_height; ++row)
        {
            for(int col = 0; col < m_condensed_width; ++col)
            {
                const SDL_Color& color_value = 
                    noise_map.at((row * m_condensed_width) + col);

                // Set the pixel value of the Surface at this location.
                surface_pixels[(row * m_condensed_width) + col] = 
                    SDL_MapRGBA(noise_surface->format, color_value.r, 
                        color_value.g, color_value.b, 255);
            }
        }

        if(surface_is_locked)
        {
            SDL_UnlockSurface(noise_surface);
        }

        m_col_cond_noise_texture = SDL_CreateTextureFromSurface(m_renderer,
            noise_surface);

        SDL_FreeSurface(noise_surface);
    }
};


class Simulator : public FrostEngine
{

public:

    Simulator() 
    {
        MenuManager::activate_menu(new NoiseViewMenu(&m_coh, _get_renderer(),
            &m_draw_handler));
    }


private:

};


int main()
{
    Simulator sim;
    sim.start();

    return 0;
}
