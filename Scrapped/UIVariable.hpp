/**
 * @file UIVariable.hpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Single Class File.
 * @version 0.1
 * @date 24-08-12
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "UIItem.hpp"


class UIVariable : public UIItem
{

public:

    // The type of the UIVariable. Determines what kind of characters can be stored in the item.
    enum VariableType
    {
        STRING, 
        INT,
        FLOAT
    };

    // Requires parameters for construction.
    UIVariable() = delete;

    UIVariable(ConsoleOutputHandler& coh, std::string& cursor_color, VariableType var_type,
        std::string name, std::string content = "", std::string default_content = "");

    void render_no_status() const final;

    void render_hovered() const final;

    void render_selected() const final;

    Status handle_input() final;

private:

    // Members

    // Function pointer.
    using input_han_func = UIItem::Status (UIVariable::*)();

    /**
     * @brief This function pointer is to the class function that will handle
     * the input from the user, depending on the type of the Variable. Since 
     * input handling is different for each Variable type, this exists to remove
     * the extra computation of an if statement to check what type the Variable
     * is each frame input needs to be handled, and instead a pointer to the 
     * respective input handling function is set on creation of the Variable, 
     * depending on what type of Variable is created.
     */
    input_han_func m_targ_input_handling_function;

    /**  If a decimal point is currently placed in the m_content for a float value. If m_var_type is 
    not FLOAT, this boolean is ignored. */
    bool m_has_decimal_point = false;

    /** If a decimal point exists in the m_default_content for a float value. If m_var_type is not
     * FLOAT, this boolean is ignored.*/
    bool m_default_has_decimal_point = false;

    /** Variable type of this object. Determines what kind of characters can be stored in the 
     * content.*/
    VariableType m_var_type;

    // Name of the Variable. This can't be edited by the user during runtime.
    const std::string m_NAME;

    // Content of the object, the variable value that the user sees and interacts with.
    std::string m_content;

    // What the content of this variable will be set to if left empty after user deselction.
    std::string m_default_content;

    
    // Methods

    
    /**
     * @brief Called once during construction. Initializes and checks the initial m_content
     * depending on the specified VariableType of this object. Such checks include checking 
     * if the type is an int or float and has letters in it, or if the type is a float if it
     * has more than one decimal point.
*/
    void _check_content_on_init();

    /** Checks m_content and m_default_content if all characters contained are valid for a Variable
     * of INT type. This means that no characters other than numbers are contained.
     */
    void _check_content_for_int_type();

    /** Checks m_content and m_default content if all characters contained are valid for a Variable
     * of FLOAT type. This means that no characters other than numbers and a single decimal point
     * are contained.
     */
    void _check_content_for_float_type();

    /** Called when the user deselects this UIVariable inside UISimulation. This checks if the
     * m_content was left blank, and if so 
     */
    void _check_handle_deselection_int_type();

    void _check_handle_deselction_float_type();

    /** Returns true if the user has deselected this item by pressing enter. If so, handle if 
     * the content is left empty. 
     */
    bool _handle_deselect();

    /**
     * @brief Specific function for handling input from the user if the 
     * Variable type is STRING
     */
    Status _handle_string_input();

    /**
     * @brief Specific function for handling input from the user if the 
     * Variable type is INT
     */
    Status _handle_int_input();

    /**
     * @brief Specific function for handling input from the user if the 
     * Variable type is FLOAT.
     */
    Status _handle_float_input();
};
