
#include <iostream>
#include <string>

#include "Menu.hpp"
#include "MenuManager.hpp"
#include "UISimulationContainers.hpp"
#include "UIChoice.hpp"
#include "UIButton.hpp"
#include "UIIntVariable.hpp"
#include "Json.hpp"
#include "EventHandler.hpp"
#include "FrostEngine.hpp"
#include "UISimulationTools.hpp"
#include "InputHandler.hpp"


class SettingsMenu : public Menu
{

public:

    SettingsMenu(ConsoleOutputHandler* coh) : Menu("SETTINGS") 
    {
        // init_data_json = &EventHandler::invoke_event<const json&>("GET_INIT_DATA");
        
        init_data_json = &EventHandler::invoke_event<const json&>("GET_INIT_DATA");

        m_font_choice = std::move(UIChoice(coh, &m_cursor_color, "Font Path", {}));

        // Add each font path to the font UIChoice content.
        for(const std::string& _str : coh->get_available_font_paths()) m_font_choice.add_choice(ColorString {_str, "White"});

        m_save_button = UIButton<SettingsMenu> (coh, &m_cursor_color, "Save", "Green", this,
            &SettingsMenu::_save_settings);

        m_menu_panel.coh = coh;
        m_menu_panel.cursor_color = &m_cursor_color;

        m_menu_panel = std::move(UIPanelContainer(coh, &m_cursor_color));
        m_menu_panel.content.push_back(&m_font_choice);
        m_menu_panel.content.push_back(&m_save_button);
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

    // UI Variable for modifying application screen width.
    UIIntVariable screen_width;

    // UI Variable for modifying application screen height.
    UIIntVariable screen_height;

    // Choice for selecting the font path.
    UIChoice m_font_choice;

    // Button for saving settings to disk. 
    UIButton<SettingsMenu> m_save_button;


    // Methods

    /** Saves the settings to disk. */
    void _save_settings()
    {
        json settings = EventHandler::invoke_event<const json&>("GET_INIT_DATA");

        settings.at("font_path") = m_font_choice.get_choice().content;

        // Save the settings to disk using the Engine through the EventHandler.
        EventHandler::invoke_event<void, const json&>("SET_INIT_DATA", settings);
    }
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

    void _user_update() final
    {
        m_coh.set_cursor(70, 0);
        m_coh.add_ch('>');
    }

    // Methods
};


int main()
{
    TextFileHandler::clear_file("CrashLog.txt");

    Simulator sim;
    sim.start();

    return 0;
}