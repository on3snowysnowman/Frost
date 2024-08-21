#include "Component.hpp"

#include <iostream>

class FirstComp : public Component<FirstComp>
{

public:

    FirstComp() : Component<FirstComp>() {}

};

class SecondComp : public Component<SecondComp>
{
public:

    SecondComp() : Component<SecondComp>() {}
};


int main()
{
    FirstComp one;
    SecondComp two;
    SecondComp three;

    std::cout << one.get_id() << '\n' << two.get_id() << "\n" << three.get_id() << '\n';

    return 0;
}