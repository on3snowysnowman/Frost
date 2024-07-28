#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "TextureHandler.hpp"


class Menu;


/** @brief Tracks all Menus that are created along with handling and processing Menus that are active.
 * 
 * For a Menu's update() method to be processed each frame, it needs to be active. To activate a 
 * Menu, use the activate_menu() method. Deactivated Menus will be ignored, so the 
 * deactivate_menu() method should be called when a Menu should be put to "sleep".
 * 
 * When a Menu is activated, the MenuManager calls its start() method. 
 */
class MenuManager
{

public:

    MenuManager();

    MenuManager(TextureHandler* tex_handler);

    /** Registers a Menu with the MenuManager using its unique ID. This method is handled 
     * internally by each Menu instance when it's created, so users should not need to call this 
     * method.
     * 
     * @param m Menu to register.
     * @param id Unique ID of the Menu.
    */
    static void _register_menu(Menu* m, std::string id);

private:

    static std::vector<Menu*> active_menus;

    static std::unordered_map<const std::string, Menu*> all_menus;

    TextureHandler* m_tex_handler;
};
