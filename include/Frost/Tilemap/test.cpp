#include "Component.hpp"

#include <iostream>

class FirstComp : public Component
{

public:

    FirstComp() {}

};

class SecondComp : public Component
{
public:

    SecondComp() {}
};


int main()
{
    FirstComp one;
    SecondComp two;
    SecondComp three;

    std::cout << get_component_id<FirstComp>() << '\n' << get_component_id<SecondComp>() << '\n';
    return 0;
}