#define SDL_MAIN_HANDLED
#define FROST_DEBUG

#include "FrostEngine.hpp"
#include "TimeObserver.hpp"


class FrostTester : public FrostEngine
{

public:

    FrostTester() : FrostEngine() {}

private:


};

#include <unordered_map>
int main()
{
    FrostTester frost_tester;
    frost_tester.start();

    return 0;
}