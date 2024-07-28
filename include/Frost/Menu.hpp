#pragma once

#include <string>
#include <cstdint>


class MenuManager;


/**
 * @brief Provides a base framework for modular user created Menu classes to inherit from and 
 * provide unique behavior each frame.
 * 
 * The Menu class is the main building block of Frost's simulation. User created classes 
 * that inherit from this class, can override the start() and update() method to provide
 * functionality that is automatically processed each frame Frost simulated. Frost contains 
 * a MenuManager that internally tracks and handles all Menus that are created, along with which 
 * Menu is active or not active.
 * 
 * Menus that are active are processed each frame. The start() method is called each time a Menu
 * is activated. The update() method is automatically called each frame by the Frost engine if the
 * Menu is activated. To activate a Menu, use the MenuManager's activate_menu() method. 
 */
class Menu
{

public:

    // Menu requires construction with a string ID.
    Menu() = delete;

    Menu(std::string id);

    /** Called when this Menu is activated by the MenuManager. */
    virtual void start();

    /** Called each frame of Frost simulation if this Menu is active. */
    virtual void update();

    /** Returns this Menu's ID.  */
    const std::string& get_id() const;

protected:

    // Members

    const std::string m_ID;


    // Methods

    /** Deactivates this Menu. */
    void _deactivate_menu() const;

};
