#define SDL_MAIN_HANDLED

#include "FrostEngine.hpp"


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