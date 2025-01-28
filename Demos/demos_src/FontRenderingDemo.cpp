
#include "FrostEngine.hpp"
#include "Menu.hpp"
#include "ConsoleOutputHandler.hpp"
#include "InputHandler.hpp"
#include "MenuManager.hpp"
#include "TextFileHandler.hpp"
#include "TimeObserver.hpp"

#include <iostream>

class FontRenderingMenu : public Menu
{

public:

    FontRenderingMenu() : Menu("DEMO")
    { 
        m_coh = nullptr;
    }

    FontRenderingMenu(ConsoleOutputHandler* coh) : Menu("DEMO")
    {
        m_coh = coh;
    }

    void start() final
    {
        m_current_font_path = m_coh->get_current_font_path();
        m_current_font_size_readable = std::to_string(m_coh->get_font_point_size());
        regenerate_output();
    }

    void update() final
    {
        if(InputHandler::is_key_pressed_and_available(SDLK_RIGHT))
        {
            switch_to_next_font();
            InputHandler::block_key_until_released(SDLK_RIGHT);
        }

        else if(InputHandler::is_key_pressed_and_available(SDLK_LEFT))
        {
            switch_to_prev_font();
            InputHandler::block_key_until_released(SDLK_LEFT);
        }

        else if(InputHandler::is_key_pressed_and_available(SDLK_UP))
        {
            increase_font_size();
            InputHandler::delay_key(SDLK_UP);
        }

        else if(InputHandler::is_key_pressed_and_available(SDLK_DOWN))
        {
            decrease_font_size();
            InputHandler::delay_key(SDLK_DOWN);
        }

        m_coh->add_str(m_output);
    }

private:

    // Current index inside the available font's vector grabbed from the ConsoleOutputHandler.
    uint8_t m_current_font_index {};

    // Path to the font that is currently active.
    std::string m_current_font_path;

    // Readable integer representing the current font size.
    std::string m_current_font_size_readable;

    // Text that is displayed each frame. 
    std::string m_output;

    ConsoleOutputHandler* m_coh;

    // Attempts to switch to the next font in the list of available fonts.
    void switch_to_next_font()
    {
        const std::vector<std::string>& available_font_paths = m_coh->get_available_font_paths();

        m_current_font_index = (m_current_font_index + 1) % available_font_paths.size();

        m_coh->set_font_path(available_font_paths.at(m_current_font_index));

        m_current_font_path = available_font_paths.at(m_current_font_index);

        regenerate_output();
    }

    // Attemps to switch to the previous font in the list of available fonts.
    void switch_to_prev_font()
    {
        const std::vector<std::string>& available_font_paths = m_coh->get_available_font_paths();

        if(m_current_font_index == 0) m_current_font_index = available_font_paths.size() - 1;
        else --m_current_font_index;
        
        m_coh->set_font_path(available_font_paths.at(m_current_font_index));

        m_current_font_path = available_font_paths.at(m_current_font_index);

        regenerate_output();
    }

    // Attempts to increase the font size.
    void increase_font_size()
    {
        const uint8_t CURRENT_FONT_SIZE = m_coh->get_font_point_size();

        if(CURRENT_FONT_SIZE < 40) m_coh->set_font_size(CURRENT_FONT_SIZE + 1);

        m_current_font_size_readable = std::to_string(m_coh->get_font_point_size());

        regenerate_output();
    }

    // Attemps to decrease the font size.
    void decrease_font_size()
    {
        const uint8_t CURRENT_FONT_SIZE = m_coh->get_font_point_size();

        if(CURRENT_FONT_SIZE > 11) m_coh->set_font_size(CURRENT_FONT_SIZE - 1);

        m_current_font_size_readable = std::to_string(m_coh->get_font_point_size());

        regenerate_output();
    }

    // Regenerates the output that is displayed each frame.
    void regenerate_output()
    {
        m_output.clear();

        m_output.append("The quick brown fox jumps over the lazy dog.\n\n");
        m_output.append(std::string(RENDERABLE_CHARACTERS) + "\n\nFont: ");
        m_output.append(m_current_font_path + "\nFont size: ");
        m_output.append(m_current_font_size_readable);
    }
};


class Simulator : public FrostEngine
{

public:

    Simulator() 
    {
        font_render_menu = FontRenderingMenu(&m_coh);
    
        MenuManager::activate_menu(&font_render_menu);
    }

private:

    FontRenderingMenu font_render_menu;
};


int main()
{
    TextFileHandler::clear_file("CrashLog.txt");

    Simulator sim;

    sim.start();

    return 0;
}
