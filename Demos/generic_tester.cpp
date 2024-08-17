#define SDL_MAIN_HANDLED

#include "FrostEngine.hpp"

#include <SDL2/SDL.h>
class FrostTester : public FrostEngine
{

public:

    FrostTester() : FrostEngine() {}

private:


};


int main()
{
    FrostTester frost_tester;
    frost_tester.start();
    return 0;
}
