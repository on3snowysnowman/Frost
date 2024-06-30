#define SDL_MAIN_HANDLED

#include "FrostEngine.h"

class FrostTester : public FrostEngine
{

public:

    FrostTester() : FrostEngine() {}

private:


};

#include <string>
#include <iostream>
int main()
{
    FrostTester frost_tester;

    return 0;
}