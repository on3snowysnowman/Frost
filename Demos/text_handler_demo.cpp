#define SDL_MAIN_HANDLED

#include "FrostEngine.hpp"
#include "Menu.hpp"
#include "MenuManager.hpp"
#include "TextHandler.hpp"

class DemoMenu : public Menu
{

public:

    DemoMenu() : Menu("DemoMenu") {}

    void start() final {}

    void update() final
    {
        
    }

private:

    TextHandler m_t_handler;
};


int main()
{
    FrostEngine frost;
    frost.start();

    return 0;
}