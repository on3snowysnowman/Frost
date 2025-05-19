
#include <iostream>
#include <vector>
#include <string>

#include <SDL_image.h>

#include <FrostEngine.hpp>
#include <Fr_Random.hpp>
#include <Fr_IntLimits.hpp>
#include <Menu.hpp>
#include <MenuManager.hpp>
#include <InputHandler.hpp>
#include <NoiseGenerator.hpp>
#include <DrawHandler.hpp>
#include <UIIntVariable.hpp>
#include <UIFloatVariable.hpp>
#include <UISimulationContainers.hpp>
#include <UISimulationTools.hpp>


class Simulator : public FrostEngine 
{

public:

    Simulator() 
    {
        _init();
    }

private:

    // Members

    float m_noise_x_offset = 0;
    float m_noise_y_offset = 0;

    //  - Noise configuration.

    int m_octaves;

    float m_exponent = 1;
    float m_threshold = 0.0f;

    float m_lacunarity;
    float m_gain;

    float m_frequency = 0.01;

    uint16_t m_noise_width;
    uint16_t m_noise_height;

    uint16_t m_cond_noise_width;
    uint16_t m_cond_noise_height;


    int m_screen_width;
    int m_screen_height;

    SDL_Rect m_noise_src_dim;
    SDL_Rect m_noise_dest_dim;
    SDL_Rect m_cond_noise_src_dim;
    SDL_Rect m_cond_noise_dest_dim;

    std::string m_cursor_color = "Blue";

    NoiseGenerator m_noise_gen;

    SDL_Texture* m_noise_texture;
    SDL_Texture* m_cond_noise_texture;


    // - UI Elements

    UIIntVariable m_noise_width_UI;
    UIIntVariable m_noise_height_UI;
    UIIntVariable m_cond_noise_width_UI;
    UIIntVariable m_cond_noise_height_UI;
    UIIntVariable m_octaves_UI;

    UIFloatVariable m_lacunarity_UI;
    UIFloatVariable m_gain_UI;

    UIPanelContainer m_panel;


    // Methods

    /**
     * @brief Initialize class members / components.
     */
    void _init()
    {
        m_screen_width = get_screen_width();
        m_screen_height = get_screen_height();

        m_noise_gen.get_noise_generator().SetSeed(0);
        m_noise_gen.get_noise_generator().SetFractalType(
            FastNoiseLite::FractalType_FBm);
            

        // Construct UI elements.

        m_noise_width_UI = UIIntVariable(&m_coh, &m_cursor_color, 
            "Noise width");
        m_noise_height_UI = UIIntVariable(&m_coh, &m_cursor_color,
            "Noise Height");
        m_cond_noise_width_UI = UIIntVariable(&m_coh, &m_cursor_color,
            "Condensed Width");
        m_cond_noise_height_UI = UIIntVariable(&m_coh, &m_cursor_color,
            "Condensed Height");

        m_octaves_UI = UIIntVariable(&m_coh, &m_cursor_color,
            "Octaves");
        m_lacunarity_UI = UIFloatVariable(&m_coh, &m_cursor_color,
            "Lacunarity");
        m_gain_UI = UIFloatVariable(&m_coh, &m_cursor_color,
            "Gain");
        
        m_noise_width_UI.content = "100";
        m_noise_height_UI.content = "100";
        m_cond_noise_width_UI.content = "10";
        m_cond_noise_height_UI.content = "10";
        m_octaves_UI.content = "3";
        m_lacunarity_UI.content = "2.0";
        m_gain_UI.content = "0.5";

        m_panel = UIPanelContainer(&m_coh, &m_cursor_color);

        m_panel.content.push_back(&m_noise_width_UI);
        m_panel.content.push_back(&m_noise_height_UI);
        m_panel.content.push_back(&m_cond_noise_width_UI);
        m_panel.content.push_back(&m_cond_noise_height_UI);
        m_panel.content.push_back(&m_octaves_UI);
        m_panel.content.push_back(&m_lacunarity_UI);
        m_panel.content.push_back(&m_gain_UI);

        // End of UI element construction. 

        _generate_new_noise();
    }

    void _init_texture_dimensions()
    {
        // Noise Dimensions

        int noise_scale_factor = m_screen_height / (m_noise_height * 2);

        m_noise_src_dim.x = 0;
        m_noise_src_dim.y = 0;
        m_noise_src_dim.w = m_noise_width;
        m_noise_src_dim.h = m_noise_height;

        m_noise_dest_dim.x = 10;
        m_noise_dest_dim.y = 10;
        m_noise_dest_dim.w = m_noise_width * noise_scale_factor;
        m_noise_dest_dim.h = m_noise_height * noise_scale_factor;

        // Condensed Noise Dimensions

        m_cond_noise_src_dim.x = 0;
        m_cond_noise_src_dim.y = 0;
        m_cond_noise_src_dim.w = m_cond_noise_width;
        m_cond_noise_src_dim.h = m_cond_noise_height;

        m_cond_noise_dest_dim.x = 10;
        m_cond_noise_dest_dim.y = m_noise_dest_dim.y + m_noise_dest_dim.h + 10;
        m_cond_noise_dest_dim.w = m_noise_dest_dim.w;
        m_cond_noise_dest_dim.h = m_noise_dest_dim.h;
    }

    void _configure_noise_generation()
    {
        // Config underlying generator.
 
        FastNoiseLite& underlying_noise_generator = 
            m_noise_gen.get_noise_generator();

        underlying_noise_generator.SetFrequency(m_frequency);
        underlying_noise_generator.SetFractalOctaves(std::stoi(
            m_octaves_UI.content));
        underlying_noise_generator.SetFractalLacunarity(std::stof(
            m_lacunarity_UI.content));
        underlying_noise_generator.SetFractalGain(std::stof(
            m_gain_UI.content));

        // Config generator

        m_noise_gen.set_offset(m_noise_x_offset, m_noise_y_offset);

        // Fetch values from UI Elements.

        m_noise_width = std::stoi(m_noise_width_UI.content);
        m_noise_height = std::stoi(m_noise_height_UI.content);
        m_cond_noise_width = std::stoi(m_cond_noise_width_UI.content);
        m_cond_noise_height = std::stoi(m_cond_noise_height_UI.content);


        _init_texture_dimensions();
    }

    void _user_update() final 
    {
        _handle_input();

        m_draw_handler.draw(m_noise_texture, m_noise_src_dim, 
            m_noise_dest_dim);
        m_draw_handler.draw(m_cond_noise_texture, m_cond_noise_src_dim,
            m_cond_noise_dest_dim);

        m_coh.set_cursor_x(50);
        m_coh.set_anchor(50);
        m_coh.add_str("Exponent: ");
        m_coh.add_str(std::to_string(m_exponent) + '\n');
        m_coh.add_str("Threshold: ");
        m_coh.add_str(std::to_string(m_threshold) + '\n');
        m_coh.add_new_line();
        Frost::simulate_UI_panel(m_panel);

    }

    void _handle_input()
    {
        bool generate_new_noise = false;

        for(const Key& key : InputHandler::get_pressed_and_available_keys())
        {
            switch(key)
            {
                case SDLK_SPACE:

                    _get_new_seed();
                    generate_new_noise = true;
                    InputHandler::block_key_until_released(SDLK_SPACE);
                    break;

                case SDLK_RIGHT:    

                    m_noise_x_offset += m_noise_width * .01f;
                    generate_new_noise = true;
                    break;
                
                case SDLK_LEFT:    

                    m_noise_x_offset -= m_noise_width * .01f;
                    generate_new_noise = true;
                    break;
                
                case SDLK_UP:    

                    m_noise_y_offset -= m_noise_height * .01f;
                    generate_new_noise = true;
                    break;
                
                case SDLK_DOWN:    

                    m_noise_y_offset += m_noise_height * .01f;
                    generate_new_noise = true;
                    break;

                case SDLK_LEFTBRACKET:

                    m_frequency *= 1.01;
                    generate_new_noise = true;
                    break;

                case SDLK_RIGHTBRACKET: 

                    m_frequency *= 0.99;
                    generate_new_noise = true;
                    break;

                case SDLK_EQUALS:

                    m_exponent += (m_exponent < 10.0) * .5;
                    generate_new_noise = true;
                    InputHandler::block_key_until_released(SDLK_EQUALS);
                    break;

                case SDLK_MINUS:

                    m_exponent -= (m_exponent > 0.5) * .5;
                    generate_new_noise = true;
                    InputHandler::block_key_until_released(SDLK_MINUS);
                    break;

                case SDLK_COMMA:

                    m_threshold -= (m_threshold > -1) * .02;
                    generate_new_noise = true;
                    InputHandler::delay_key(SDLK_COMMA);
                    break;

                case SDLK_PERIOD:

                    m_threshold += (m_threshold < 1) * .02;
                    generate_new_noise = true;
                    InputHandler::delay_key(SDLK_PERIOD);
                    break;
            }
        }

        if(generate_new_noise) _generate_new_noise();
    }

    void _get_new_seed()
    {
        m_noise_gen.get_noise_generator().SetSeed(
            FrostRandom::get_random_num<uint64_t>(0, UINT64_MAX));
    }

    void _generate_new_noise()
    {
        _configure_noise_generation();
            
        std::vector<float> noise = m_noise_gen.generate_full_noise_map(m_noise_width, 
            m_noise_height);

        for(float& value : noise)
        {
            // Squares the value by 2, and multiplies it by -1 if it was 
            // originally negative to maintain its sign since squaring it 
            // removes it.
            value = std::pow(std::abs(value), m_exponent) * (1 - 2 * (value < 0));
        }
        
        _create_noise_texture(noise);

        std::vector<float> condensed_noise = m_noise_gen.condense_noise_map(
            noise, m_noise_width, m_noise_height, m_cond_noise_width, 
            m_cond_noise_height);

        std::vector<SDL_Color> colored_condensed_noise = 
            m_noise_gen.colorize_noise_map(condensed_noise, 
            {
                {m_threshold, SDL_Color{0, 0, 200, 255}},
                {1, SDL_Color{0, 0, 0, 255}}
            });

        _create_condensed_noise_texture(colored_condensed_noise);
    }

    void _create_noise_texture(const std::vector<float>& noise) 
    {
        if(m_noise_texture) SDL_DestroyTexture(m_noise_texture);

        SDL_Surface* noise_surface = SDL_CreateRGBSurfaceWithFormat(0, 
            m_noise_width, m_noise_height, 32, SDL_PIXELFORMAT_ARGB32);
        
        // Get the array of pixels of the surface.
        uint32_t* pixels = static_cast<uint32_t*>(noise_surface->pixels);

        bool surface_is_locked = false;

        if(SDL_MUSTLOCK(noise_surface))
        {
            SDL_LockSurface(noise_surface);
            surface_is_locked = true;
        }

        for(uint32_t i = 0; i < m_noise_width * m_noise_height; ++i)
        {
            // Convert the noise value that ranges from [-1, 1] to a grayscale 
            // value that ranges from [0, 254]
            uint32_t grayscale_value = (noise.at(i) + 1) * 122;

            pixels[i] = SDL_MapRGBA(noise_surface->format, grayscale_value,
                grayscale_value, grayscale_value, 255);
        }

        if(surface_is_locked)
        {
            SDL_UnlockSurface(noise_surface);
        }

        m_noise_texture = SDL_CreateTextureFromSurface(_get_renderer(),
            noise_surface);
        
        // IMG_SavePNG(noise_surface, "Noise.png");

        SDL_FreeSurface(noise_surface);
    }

    void _create_condensed_noise_texture(const std::vector<SDL_Color>& noise) 
    {
        if(m_cond_noise_texture) SDL_DestroyTexture(m_cond_noise_texture);

        SDL_Surface* noise_surface = SDL_CreateRGBSurfaceWithFormat(0, 
            m_cond_noise_width, m_cond_noise_height, 32, SDL_PIXELFORMAT_ARGB32);
        
        // Get the array of pixels of the surface.
        uint32_t* pixels = static_cast<uint32_t*>(noise_surface->pixels);

        bool surface_is_locked = false;

        if(SDL_MUSTLOCK(noise_surface))
        {
            SDL_LockSurface(noise_surface);
            surface_is_locked = true;
        }

        for(uint32_t i = 0; i < m_cond_noise_width * m_cond_noise_height; ++i)
        {
            // Convert the noise value that ranges from [-1, 1] to a grayscale 
            // value that ranges from [0, 254]
            // uint32_t grayscale_value = (noise.at(i) + 1) * 122;
            const SDL_Color& color = noise.at(i);

            pixels[i] = SDL_MapRGBA(noise_surface->format, color.r, color.g,
                color.b, 255);

            // pixels[i] = SDL_MapRGBA(noise_surface->format, grayscale_value,
            //     grayscale_value, grayscale_value, 255);
        }

        if(surface_is_locked)
        {
            SDL_UnlockSurface(noise_surface);
        }

        m_cond_noise_texture = SDL_CreateTextureFromSurface(_get_renderer(),
            noise_surface);
        
        SDL_FreeSurface(noise_surface);
    }
};


int main()
{

    Simulator sim;
    sim.start();

    return 0;
}
