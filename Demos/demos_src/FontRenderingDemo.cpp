
#include "FrostEngine.hpp"
#include "Menu.hpp"
#include "ConsoleOutputHandler.hpp"
#include "InputHandler.hpp"
#include "MenuManager.hpp"

#include <iostream>

class DemoMenu : public Menu
{

public:

    DemoMenu() : Menu("DEMO")
    { 
        m_coh = nullptr;
    }

    DemoMenu(ConsoleOutputHandler* coh) : Menu("DEMO")
    {
        m_coh = coh;

        // m_coh->set_font_path("/home/joel/Documents/Code/Frost/Demos/data/fonts/")

        m_coh->set_font_size(12);
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

        m_coh->add_str("The quick brown fox jumps over the lazy dog.");
    }

private:

    uint8_t current_font_index {};

    std::string output;

    ConsoleOutputHandler* m_coh;

    void switch_to_next_font()
    {
        const std::vector<std::string>& available_font_paths = m_coh->get_available_font_paths();

        current_font_index = (current_font_index + 1) % available_font_paths.size();

        std::cout << "Setting font: " << available_font_paths.at(current_font_index) << '\n';

        m_coh->set_font_path(available_font_paths.at(current_font_index));
    }

    void switch_to_prev_font()
    {
        const std::vector<std::string>& available_font_paths = m_coh->get_available_font_paths();

        if(current_font_index == 0) current_font_index = available_font_paths.size() - 1;
        else --current_font_index;
        
        std::cout << "Setting font: " << available_font_paths.at(current_font_index) << '\n';

        m_coh->set_font_path(available_font_paths.at(current_font_index));
    }
};


class Simulator : public FrostEngine
{

public:

    Simulator() 
    {
        d_m = DemoMenu(&m_coh);
    
        MenuManager::activate_menu(&d_m);
    }



private:

    DemoMenu d_m;
};


int main()
{
    Simulator sim;

    sim.start();

    return 0;
}
