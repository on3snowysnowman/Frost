#define SDL_MAIN_HANDLED

#include "FrostEngine.hpp"
#include "Menu.hpp"
#include "UISimulationContainers.hpp"
#include "UISimulationTools.hpp"
#include "MenuManager.hpp"
#include "UIText.hpp"
#include "UIButton.hpp"
#include "UIStringVariable.hpp"
#include "UIIntVariable.hpp"
#include "UIFloatVariable.hpp"
#include "UIChoice.hpp"
#include "UITextList.hpp"
#include "UIList.hpp"

#include <iostream>
class DemoMenu : public Menu
{

public:

    DemoMenu(ConsoleOutputHandler& coh, std::string& cursor_color) : Menu("DemoMenu"), m_cursor_color(cursor_color),
        m_coh(coh) 
    {
        pan_cont.coh = &m_coh;
        pan_cont.cursor_color = &m_cursor_color;

        pan_cont.content.push_back(new UIText(m_coh, m_cursor_color, "Text"));
        pan_cont.content.push_back(new UIStringVariable(m_coh, m_cursor_color, "String Variable"));
        pan_cont.content.push_back(new UIIntVariable(m_coh, m_cursor_color, "Int Variable"));
        pan_cont.content.push_back(new UIFloatVariable(m_coh, m_cursor_color, "Float Variable"));
        pan_cont.content.push_back(new UIChoice(m_coh, m_cursor_color, "Choice", 
            {ColorString{"Choice1", "Green"}, ColorString{"Choice2", "Purple"}}));
        pan_cont.content.push_back(new UITextList(m_coh, m_cursor_color, "Text List", {}));
        pan_cont.content.push_back(new UIList(m_coh, m_cursor_color, "List", 
            {new UIText(m_coh, m_cursor_color), 
            new UIStringVariable(m_coh, m_cursor_color, "String Variable"),
            new UITextList(m_coh, m_cursor_color, "Text List", {})}));
        pan_cont.content.push_back(new UIButton<DemoMenu>(coh, m_cursor_color, "Button", 
            "Green", this, &DemoMenu::test_method));
    }

    void start() final 
    {
        pan_cont.reset();
    }

    void update() final 
    {
        m_coh.add_str("Panel: \n");
        Frost::simulate_UI_panel(pan_cont);
    }

private:

    // Members

    // Color of the cursor inside UISimulationTools' simulation.
    std::string& m_cursor_color;

    UIPanelContainer pan_cont;

    ConsoleOutputHandler& m_coh;


    // Methods

    void test_method()
    {
        std::cout << "Test method invoked!\n";
    }
};




class FrostTester : public FrostEngine
{

public:

    FrostTester() : FrostEngine() 
    {
        demo_m = new DemoMenu(m_coh, cursor_color);

        MenuManager::activate_menu(demo_m);
    }

private:

    std::string cursor_color = "Blue";

    DemoMenu* demo_m;
};


int main()
{
    FrostTester frost_tester;
    frost_tester.start();

    return 0;
}
