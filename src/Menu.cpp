#include "Menu.hpp"

#include "MenuManager.hpp"

// Constructors / Deconstructor

Menu::Menu(std::string id) : m_ID(id)
{
    MenuManager::_register_menu(this, m_ID);
}


// Public

void Menu::start() {}

void Menu::update() {}

const std::string& Menu::get_id() const { return m_ID; }


// Private

void Menu::_deactivate_menu() const { MenuManager::deactivate_menu(this); }
