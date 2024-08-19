#include "MenuManager.hpp"


// Static Members

std::list<Menu*> MenuManager::active_menus;

std::unordered_map<std::string, Menu*>* MenuManager::all_menus =
    new std::unordered_map<std::string, Menu*>;


// Public

void MenuManager::update_active_menus()
{
    for(Menu* m : active_menus) m->update();
}

void MenuManager::_register_menu(Menu* m, const std::string& id)
{
    all_menus->emplace(id, m);
}

void MenuManager::activate_menu(Menu* m) { active_menus.push_back(m); }

void MenuManager::activate_menu(std::string id) { activate_menu(all_menus->at(id)); }

void MenuManager::deactivate_menu(const Menu* m)
{
    // Iterate through each activated Menu to find the one to delete.

    for(std::list<Menu*>::const_iterator it = active_menus.begin();
        it != active_menus.end(); ++it)
    {
        if((*it) == m)
        {
            active_menus.erase(it);
            return;
        }
    }
}

void MenuManager::deactivate_menu(std::string id) { deactivate_menu(all_menus->at(id)); }


// Private
