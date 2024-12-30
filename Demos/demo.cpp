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

    DemoMenu(ConsoleOutputHandler* coh, TextRenderingHandler* text_ren_handler, 
        SpriteHandler* sprite_handler, const double& frame_time) 
        : Menu("DemoMenu")
    {
        m_text_ren_handler = text_ren_handler;
        m_coh = coh;
        m_sprite_handler = sprite_handler;
    }

    void start() final {}

    void update() final 
    {
        m_coh->add_str("Text");
        m_coh->move_cursor(1, 0);
        m_coh->add_ch('|');
    }

private:

    TextRenderingHandler* m_text_ren_handler;
    ConsoleOutputHandler* m_coh;
    SpriteHandler* m_sprite_handler;
};

class DemoEngine : public FrostEngine
{

public:

    DemoEngine() : FrostEngine() 
    {
        m_sprite_handler.set_sprite_scale_factor(3.0f);

        m_demo_menu = new DemoMenu(&m_coh, &m_text_ren_handler, &m_sprite_handler,
            get_frame_time_reference());

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



