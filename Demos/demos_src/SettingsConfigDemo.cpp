
#include <iostream>
#include <string>

#include "Menu.hpp"
#include "MenuManager.hpp"
#include "UISimulationContainers.hpp"
#include "UIChoice.hpp"
#include "UIButton.hpp"
#include "UIIntVariable.hpp"
#include "Json.hpp"
#include "FrostEngine.hpp"
#include "EventHandler.hpp"
#include "UISimulationTools.hpp"
#include "InputHandler.hpp"
#include "Fr_StringManip.hpp"


class SettingsMenu : public Menu
{

public:

    SettingsMenu(ConsoleOutputHandler* coh) : Menu("SETTINGS") 
    {
        init_data_json = &EventHandler::invoke_event<const json&>("GET_INIT_DATA");

        m_native_screen_width = EventHandler::invoke_event<int>("GET_NATIVE_SCREEN_WIDTH");
        m_native_screen_height = EventHandler::invoke_event<int>("GET_NATIVE_SCREEN_HEIGHT");

        m_font_choice = UIChoice(coh, &m_cursor_color, "Font Path", {});

        const std::vector<std::string>& font_paths = coh->get_available_font_paths();

        // The active font fetched from the data settings.
        const std::string active_font = init_data_json->at("font_path").get<std::string>();

        // Iterate through each font path.
        for(uint8_t i = 0; i < font_paths.size(); ++i)
        {
            // Add this font path to the UI Font Choice
            m_font_choice.add_choice(ColorString{font_paths[i], "White"});

            // This font path is the active font path. Update the UI Font 
            // Choice to be set to this font path.
            if(font_paths[i] == active_font)
            {
                m_font_choice.m_selected_index = i;
                m_font_choice.m_cursor_index = i;
            }
        }

        m_screen_width = UIIntVariable(coh, &m_cursor_color, "Screen Width", 
            std::to_string(static_cast<int>(init_data_json->at("screen_width"))), "500");
        m_screen_height = UIIntVariable(coh, &m_cursor_color, "Screen Height", 
            std::to_string(static_cast<int>(init_data_json->at("screen_height"))), "500");

        m_save_button = UIButton<SettingsMenu> (coh, &m_cursor_color, "Save", "Green", this,
            &SettingsMenu::_save_settings);

        m_menu_panel.coh = coh;
        m_menu_panel.cursor_color = &m_cursor_color;

        m_menu_panel = UIPanelContainer(coh, &m_cursor_color);
        m_menu_panel.content.push_back(&m_font_choice);
        m_menu_panel.content.push_back(&m_screen_width);
        m_menu_panel.content.push_back(&m_screen_height);
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

    uint16_t m_native_screen_width;
    uint16_t m_native_screen_height;

    std::string m_cursor_color = "Blue"; 

    // Json object containing the data of the init file.
    const json* init_data_json;

    // Panel container for the menu items for menu simulation.
    UIPanelContainer m_menu_panel; 

    // UI Variable for modifying application screen width.
    UIIntVariable m_screen_width;

    // UI Variable for modifying application screen height.
    UIIntVariable m_screen_height;

    // Choice for selecting the font path.
    UIChoice m_font_choice;

    // Button for saving settings to disk. 
    UIButton<SettingsMenu> m_save_button;


    // Methods

    /** Saves the settings to disk. */
    void _save_settings()
    {
        json settings = EventHandler::invoke_event<const json&>("GET_INIT_DATA");

        // Fetch screen dimensions.
        uint16_t new_screen_width = Frost::str_to_uint16(m_screen_width.m_content);
        uint16_t new_screen_height = Frost::str_to_uint16(m_screen_height.m_content);

        // Check if the screen dimensions are invalid.
        if(new_screen_width < 500 || new_screen_width > m_native_screen_width)
            new_screen_width = m_native_screen_width;
        if(new_screen_height < 500 || new_screen_height > m_native_screen_height)
            new_screen_height = m_native_screen_height;

        settings.at("screen_width") = new_screen_width;
        settings.at("screen_height") = new_screen_height;

        // Fetch font.
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

    // void _user_update() final
    // {
    //     m_coh.set_cursor(70, 0);
    //     m_coh.add_ch('>');
    // }

    // Methods
};


int main()
{
    TextFileHandler::clear_file("CrashLog.txt");

    Simulator sim;
    sim.start();

    return 0;
}