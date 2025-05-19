
#include <iostream>

#include <Fr_Random.hpp>


int main()
{
    for (size_t i = 0; i < 4; i++)
        std::cout << int(FrostRandom::get_random_num<uint8_t>(1, 100)) << '\n';

    return 0;
}
