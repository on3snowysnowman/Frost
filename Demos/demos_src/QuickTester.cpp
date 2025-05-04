
#include "FrostEngine.hpp"
#include "SpriteHandler.hpp"
#include "Sprite.hpp"
#include "InputHandler.hpp"
#include "Fr_Random.hpp"


class Simulator : public FrostEngine
{

public:

    Simulator() 
    {
        sprite = m_sprite_handler.create_sprite("assets/Frost_Icon.png",
            0, 0, 27 * 2, 28 * 2);

        sprite_rendering_id = m_sprite_handler.render_sprite(sprite, 10, 10);

        direction = {1, 1};
    }


private:

    void _user_update() final
    {
        if(direction_delay >= 60)
        {
            direction_delay = 0;
            direction.first = FrostRandom::get_random_num<int8_t>(-1, 1);
            direction.second = FrostRandom::get_random_num<int8_t>(-1, 1);
        }

        const SpriteInstanceData& instance_data = 
            m_sprite_handler.get_instance_data(sprite_rendering_id);

        if(instance_data.display_dimensions.x < 0)
        {
            m_sprite_handler.set_instance_position(sprite_rendering_id,
                0, instance_data.display_dimensions.y);
            
            direction.first = 1;
        }

        else if(instance_data.display_dimensions.x > 500 -(27*2))
        {
            m_sprite_handler.set_instance_position(sprite_rendering_id,
                500 - (27*2), instance_data.display_dimensions.y);
            direction.first = -1;
        }

        if(instance_data.display_dimensions.y < 0)
        {
            m_sprite_handler.set_instance_position(sprite_rendering_id,
                instance_data.display_dimensions.x, 0);

            direction.second = 1;
        }

        else if(instance_data.display_dimensions.y > 500 - (28 * 2))
        {
            m_sprite_handler.set_instance_position(sprite_rendering_id,
                instance_data.display_dimensions.x, 500 - (28 * 2));
            direction.second = -1;        
        }

        ++direction_delay;

        m_sprite_handler.modify_instance_position(sprite_rendering_id,
            direction.first * 6, direction.second * 6);
    }

    uint16_t direction_delay = 0;

    std::pair<int8_t, int8_t> direction;

    bool deleted_sprite = false;

    Sprite sprite;
    
    rendering_id sprite_rendering_id;


};

int main()
{
    Simulator sim;
    sim.start();

    return 0;
}
