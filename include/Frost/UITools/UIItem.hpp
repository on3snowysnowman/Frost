/**
 * @file UIItem.hpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Base Component for UIItems
 * @version 0.1
 * @date 24-08-12
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "ConsoleOutputHandler.hpp"


namespace Frost
{


/** Represents different states that a UIItem can exist in inside the simulation of the 
 * UIInterfaceTools' simulation. NO_STATUS means the UIItem has no status applied to it. HOVERED 
 * means that this item is currently hovered by the cursor, and SELECTED represents this item is
 * currently being selected.
*/
enum UIItemStatus
{
    NO_STATUS, 
    HOVERED,
    SELECTED
};


/** Different types of UIItems that exist. */
enum UIItemType
{
    BUTTON,
    TEXT,
    VARIABLE,
    CHOICE,
    TEXT_LIST,
    ITEM_LIST
};

};


/** Provides the base infastructure for more in depth UIItems to derive from. UIItems are used to
 * create modular and recursive interfaces for users to interact with in a text-menu system.
*/
class UIItem
{

public:

    // Requires ctor with item type specifier
    UIItem() = delete;

    UIItem(ConsoleOutputHandler& coh, Frost::UIItemType item_type);

    /** Renders this UIItem with respect to having no status. */
    virtual void render_no_status() const;

    /** Renders this UIItem with respect to being hovered by the cursor. */
    virtual void render_hovered() const;

    /** Renders this UIItem with respect to being selected.  */
    virtual void render_selected() const;

    /** Called each frame when this UIItem is selected in the UISimulationTools' simulation,
     * allows this UIItem to handle user input and make changes to itself. Returns the updated
     * status of this UIItem after it has handled the user's input. 
     */
    virtual Frost::UIItemStatus handle_input();

protected:

    // Type of this UIItem.
    const Frost::UIItemType m_ITEM_TYPE;

    ConsoleOutputHandler& m_coh;
};
