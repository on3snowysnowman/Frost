/**
 * @file MenuManager.cpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Single class implementation.
 * @version 0.1
 * @date ?
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "MenuManager.hpp"


// Static Members

std::list<Menu*> MenuManager::s_active_menus;

std::unordered_map<std::string, Menu*> MenuManager::s_all_menus;


// Public

void MenuManager::_update_active_menus()
{
    for(Menu* m : s_active_menus) m->update();
}

void MenuManager::_register_menu(Menu* m, const std::string& id)
{
    s_all_menus.emplace(id, m);
}

void MenuManager::activate_menu(Menu* m) 
{ 
    s_active_menus.push_back(m); 
    m->start();
}

void MenuManager::activate_menu(std::string id) { activate_menu(s_all_menus.at(id)); }

void MenuManager::deactivate_menu(const Menu* m)
{
    // Iterate through each activated Menu to find the one to delete.

    for(std::list<Menu*>::const_iterator it = s_active_menus.begin();
        it != s_active_menus.end(); ++it)
    {
        if((*it) == m)
        {
            s_active_menus.erase(it);
            return;
        }
    }
}

void MenuManager::deactivate_menu(std::string id) { deactivate_menu(s_all_menus.at(id)); }


// Private
