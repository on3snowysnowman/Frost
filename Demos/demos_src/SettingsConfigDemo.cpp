
#include <iostream>
#include <string>

#include "Menu.hpp"
#include "MenuManager.hpp"
#include "UISimulationContainers.hpp"
#include "UIChoice.hpp"
#include "UIButton.hpp"
#include "Json.hpp"
#include "EventHandler.hpp"
#include "FrostEngine.hpp"
#include "UISimulationTools.hpp"


class SettingsMenu : public Menu
{

public:

    SettingsMenu(ConsoleOutputHandler* coh) : Menu("SETTINGS") 
    {
        // init_data_json = &EventHandler::invoke_event<const json&>("GET_INIT_DATA");
        
        init_data_json = &EventHandler::invoke_event<const json&>("GET_INIT_DATA");

        font_choice = std::move(UIChoice(coh, &m_cursor_color, "Font Path", {}));

        // Add each font path to the font UIChoice content.
        for(const std::string& _str : coh->get_available_font_paths()) font_choice.add_choice(ColorString {_str, "White"});

        m_menu_panel.coh = coh;
        m_menu_panel.cursor_color = &m_cursor_color;

        m_menu_panel = std::move(UIPanelContainer(coh, &m_cursor_color));
        m_menu_panel.content.push_back(&font_choice);
    }

    void start() final 
    {
        m_menu_panel.reset();
    }

    void update() final 
    {
        Frost::simulate_UI_panel(m_menu_panel);
    }

private:

    // Members

    std::string m_cursor_color = "Blue"; 

    // Json object containing the data of the init file.
    const json* init_data_json;

    // Panel container for the menu items for menu simulation.
    UIPanelContainer m_menu_panel; 

    UIChoice font_choice;


    // Methods

};



class Simulator : public FrostEngine
{

public:

    Simulator()
    {
        MenuManager::activate_menu(new SettingsMenu(&m_coh));
    }

private:

    // Members


    // Methods
};


int main()
{
    TextFileHandler::clear_file("CrashLog.txt");

    Simulator sim;
    sim.start();

    return 0;
}