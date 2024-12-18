#define SDL_MAIN_HANDLED

#include <iostream>

#include <FrostEngine.hpp>
#include <Menu.hpp>
#include <MenuManager.hpp>
#include <InputHandler.hpp>
#include <Fr_StringManip.hpp>


class DemoMenu : public Menu
{

public:

    DemoMenu() : Menu("DemoMenu") {}

    DemoMenu(ConsoleOutputHandler* coh, TextRenderingHandler* text_ren_handler) 
        : Menu("DemoMenu") 
    {
        m_text_ren_handler = text_ren_handler;
        m_coh = coh;
    }

    void start() final {}

    void update() final 
    {
        Frost::handle_input_for_string_manipulation(text);

        m_coh->add_str(text);
    }

private:

    std::string text;

    TextRenderingHandler* m_text_ren_handler;
    ConsoleOutputHandler* m_coh;
};
    
#include <iostream>

class DemoEngine : public FrostEngine
{

public:

    DemoEngine() : FrostEngine() 
    {
        m_demo_menu = new DemoMenu(&m_coh, &m_text_ren_handler);

        MenuManager::activate_menu(m_demo_menu);
    }


private:

    TextRenderingHandler m_text_handler;

    DemoMenu* m_demo_menu;
};


int main()
{
    DemoEngine demo_engine;
    demo_engine.start();

    return 0;
}



