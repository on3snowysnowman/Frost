
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
        m_char_one_id = m_tilemap.register_entity('c');

        m_tilemap.add_entity_to_tilemap(m_char_one_id, 3, 3);

        _regenerate_tilemap_display();
    }

private:

    // Members

    entity_id m_char_one_id;

    std::string m_output;

    Tilemap<char, 10> m_tilemap;


    // Methods

    void _user_update() final
    {           
        if(InputHandler::is_key_pressed_and_available(SDLK_RIGHT))
        {
            m_tilemap.modify_entity_position(m_char_one_id, 1, 0);
            _regenerate_tilemap_display();
            InputHandler::delay_key(SDLK_RIGHT);
        }

        else if(InputHandler::is_key_pressed_and_available(SDLK_LEFT))
        {
            m_tilemap.modify_entity_position(m_char_one_id, -1, 0);
            _regenerate_tilemap_display();
            InputHandler::delay_key(SDLK_LEFT);
        }

        else if(InputHandler::is_key_pressed_and_available(SDLK_UP))
        {
            m_tilemap.modify_entity_position(m_char_one_id, 0, -1);
            _regenerate_tilemap_display();
            InputHandler::delay_key(SDLK_UP);
        }

        else if(InputHandler::is_key_pressed_and_available(SDLK_DOWN))
        {
            m_tilemap.modify_entity_position(m_char_one_id, 0, 1);
            _regenerate_tilemap_display();
            InputHandler::delay_key(SDLK_DOWN);

        }

        m_coh.add_str(m_output);
    }

    void _regenerate_tilemap_display()
    {
        m_output.clear();

        const Tilemap<char, 10>::content_type& contents = m_tilemap.get_contents();

        for(uint32_t i = 0; i < contents.size(); ++i)
        {
            for(uint32_t j = 0; j < contents.size(); ++j)
            {
                if(contents.at(i).at(j).size() == 0)
                {
                    m_output.push_back('.');
                    continue;
                }

                m_output.push_back(contents.at(i).at(j).front());
            }

            m_output.push_back('\n');
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