/**
 * @file Menu.cpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Single class implementation.
 * @version 0.1
 * @date ?
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "Menu.hpp"

#include "MenuManager.hpp"

// Constructors / Deconstructor

Menu::Menu() {}

Menu::Menu(std::string id)
{
    m_id = id;

    MenuManager::_register_menu(this, m_id);
}


// Public

void Menu::start() {}

void Menu::update() {}

const std::string& Menu::get_id() const { return m_id; }


// Private

void Menu::_deactivate_menu() const { MenuManager::deactivate_menu(this); }
