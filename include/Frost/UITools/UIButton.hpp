/**
 * @file UIButton.hpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Declaration for UIButton.
 * @version 0.1
 * @date 2024-08-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

 #pragma once


#include "UIItem.hpp"


template<typename T> 
class UIButton : public UIItem
{

public:

    // Requires parameters for construction.
    UIButton() = delete;

    // UIButton(ConsoleOutputHandler& coh, std::string& cursor_color, std::string name,
    //     std::string button_color, T* obj, std::function<ReturnT(T&)> callback_method) : 
    //     UIItem(coh, cursor_color, "BUTTON"), m_object_pointer(obj)
    // {
    //     m_name = name;
    //     m_button_color = button_color;

    //     m_callback_method = callback_method;
    // }

    UIButton(ConsoleOutputHandler& coh, std::string& cursor_color, std::string name,
        std::string button_color, T* obj, void(T::*callback_method)()) : 
        UIItem(coh, cursor_color, "BUTTON"), m_object_pointer(obj)
    {
        m_name = name;
        m_button_color = button_color;

        m_callback_method = callback_method;
    }

    void render_no_status() const final 
    {
        m_coh.add_str("   " + m_name, m_button_color);
    }

    void render_hovered() const final 
    {
        m_coh.add_str(" > ", m_cursor_color);
        m_coh.add_str(m_name, m_button_color);
    }

    void render_selected() const final 
    {
        // No difference between hovered and selected. 
        render_hovered();
    }

    Status handle_input() final 
    {
        // Invoke callback method
        (m_object_pointer->*m_callback_method)();

        // Deselect this button after invoking the callback method.
        return HOVERED;
    }


private:

    // Name of this button.
    std::string m_name;

    // Color of the button.
    std::string m_button_color;

    // Pointer to the object $m_callback_function belongs to.
    T* const m_object_pointer;

    // The method that is called when this button is selected in simulation.
    // std::function<ReturnT(T&)> m_callback_method;

    void(T::*m_callback_method)();

};
