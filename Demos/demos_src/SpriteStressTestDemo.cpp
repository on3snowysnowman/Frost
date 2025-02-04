
#include "FrostEngine.hpp"
#include "Menu.hpp"
#include "ConsoleOutputHandler.hpp"
#include "InputHandler.hpp"
#include "MenuManager.hpp"
#include "TextFileHandler.hpp"
#include "TimeObserver.hpp"
#include "SpriteHandler.hpp"

#include <iostream>


class Simulator : public FrostEngine
{

public:

    Simulator() 
    {
        constexpr uint32_t NUM_SPRITES = 1;

        for(uint32_t i = 0; i < NUM_SPRITES; ++i)
        {
            m_sprite_handler.flag_render(
                m_sprite_handler.create_sprite(0, 0, 27, 28, 100, 100, 
                "assets/Frost_Icon.png"));
        }
        // frost_icon = m_sprite_handler.create_sprite(0, 0, 27, 28, 100, 100, 
                // "assets/Frost_Icon.png");

        // m_sprite_handler.flag_render(frost_icon);

        last_timestamp = TimeObserver::get_time_point();
    }

private:

    void _user_update() final 
    {
        // if(direction == 1)
        // {
        //     if(m_sprite_handler.get_sprite(frost_icon).display_dimensions.x >= 500)
        //         direction = -1;
        // }

        // else
        // {
        //     if(m_sprite_handler.get_sprite(frost_icon).display_dimensions.x <= 10)
        //         direction = 1;
        // }
        
        c_time_point current_time_stamp = TimeObserver::get_time_point();

        if(TimeObserver::calculate_interval_from_timepoints(last_timestamp, current_time_stamp) > 200)
        {
            last_timestamp = current_time_stamp;
            calculate_fps();
        }

        m_coh.add_str(output);

        // m_sprite_handler.modify_sprite_position(frost_icon, direction, 0);
    }

    void calculate_fps()
    {
        uint16_t now_fps = 1000 / _get_frame_time_reference();

        // output = std::to_string(_get_frame_time_reference());
        output = std::to_string(now_fps);
    }

    int8_t direction = 1;

    sprite_id frost_icon;

    std::string output;

    c_time_point last_timestamp;
};


int main()
{
    TextFileHandler::clear_file("CrashLog.txt");

    Simulator sim;

    sim.start();

    return 0;
}
