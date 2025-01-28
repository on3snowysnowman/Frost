
#include <iostream>
#include <string>

#include "Tilemap.hpp"
#include "FrostEngine.hpp"
#include "InputHandler.hpp"
#include "TextFileHandler.hpp"


class Simulator : public FrostEngine
{

public:

    Simulator()
    {
        tilemap.add_entity(tilemap.register_entity('c'), 1, 4);
        tilemap.add_entity(tilemap.register_entity('c'), 4, 4);
        tilemap.add_entity(3, 4, 4);

        _regenerate_tilemap_display();
    }

private:

    // Members

    std::string output;

    Tilemap<char, 10> tilemap;

    // Methods

    void _user_update() final
    {           
        if(InputHandler::is_key_pressed_and_available(SDLK_RIGHT))
        {
            _regenerate_tilemap_display();
            InputHandler::block_key_until_released(SDLK_RIGHT);
        }

        else if(InputHandler::is_key_pressed_and_available(SDLK_LEFT))
        {
            _regenerate_tilemap_display();
            InputHandler::block_key_until_released(SDLK_LEFT);
        }

        else if(InputHandler::is_key_pressed_and_available(SDLK_UP))
        {
            _regenerate_tilemap_display();
            InputHandler::block_key_until_released(SDLK_UP);
        }

        else if(InputHandler::is_key_pressed_and_available(SDLK_DOWN))
        {
            _regenerate_tilemap_display();
            InputHandler::block_key_until_released(SDLK_DOWN);

        }

        m_coh.add_str(output);
    }

    void _regenerate_tilemap_display()
    {
        output.clear();

        const Tilemap<char, 10>::content_type& contents = tilemap.get_contents();

        for(uint32_t i = 0; i < contents.size(); ++i)
        {
            for(uint32_t j = 0; j < contents.size(); ++j)
            {
                if(contents.at(i).at(j).size() == 0)
                {
                    output.push_back('.');
                    continue;
                }

                output.push_back(contents.at(i).at(j).front());
            }

            output.push_back('\n');
        }
    }
};


int main()
{
    TextFileHandler::clear_file("CrashLog.txt");

    Simulator sim;
    sim.start();

    return 0;
}