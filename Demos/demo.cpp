#define SDL_MAIN_HANDLED

#include <iostream>

#include "FrostEngine.h"

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