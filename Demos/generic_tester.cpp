#define SDL_MAIN_HANDLED

#include "FrostEngine.hpp"
#include "../include/SpriteHandler.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL.h>
#include <iostream>


class FrostTester : public FrostEngine
{

public:

    FrostTester() : FrostEngine() {}

private:


};

int main()
{
    FrostTester frost_tester;
    
    return 0;
}