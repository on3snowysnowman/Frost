#pragma once

#include <string>
#include <list>
#include <unordered_map>

#include "Menu.hpp"


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

    static void update_active_menus();

    /** Registers a Menu with the MenuManager using its unique ID. This method is handled 
     * internally by each Menu instance when it's created, so users should not need to call this 
     * method.
     * 
     * @param m Menu to register.
     * @param id Unique ID of the Menu.
    */
    static void _register_menu(Menu* m, const std::string& id);

    /** Activate a Menu.
     * 
     * @param m Menu to activate.
     */
    static void activate_menu(Menu* m);

    /** Activate a Menu, using its ID as a lookup. 
     * 
     * @param id ID of the Menu to activate.
    */
    static void activate_menu(std::string id);

    /** Deactivate a Menu.
     * 
     * @param m Menu to deactivate.
     */
    static void deactivate_menu(const Menu* m);

    /** Deactivate a Menu, using its ID as a lookup.
     * 
     * @param id ID of the Menu to deactivate.
     */
    static void deactivate_menu(std::string id);

private:

    // Menus that will be updated each frame.
    static std::list<Menu*> active_menus;

    // All Menus that are tracked by the MenuManager. Menu's IDs are used as the key.
    static std::unordered_map<std::string, Menu*>* all_menus;
};
