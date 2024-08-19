#define SDL_MAIN_HANDLED

#include "FrostEngine.hpp"
#include "Menu.hpp"
#include "UISimulationContainers.hpp"
#include "UISimulationTools.hpp"
#include "MenuManager.hpp"
#include "UIText.hpp"


class DemoMenu : public Menu
{

public:

    DemoMenu(ConsoleOutputHandler& coh, std::string& cursor_color) : Menu("DemoMenu"), m_cursor_color(cursor_color),
        m_coh(coh) 
    {
        pan_cont.coh = &m_coh;
        pan_cont.cursor_color = &m_cursor_color;

        pan_cont.content.push_back(new UIText(m_coh, m_cursor_color, "Text"));
        pan_cont.content.push_back(new UIText(m_coh, m_cursor_color, "Text"));
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

    // Color of the cursor inside UISimulationTools' simulation.
    std::string& m_cursor_color;

    UIPanelContainer pan_cont;

    ConsoleOutputHandler& m_coh;
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
