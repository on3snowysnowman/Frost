#pragma once

#include <string>
#include <cstdint>

/** Pure storage struct containing basic data for a color. Contains rgb values and a name. */
struct Color
{
    Color() {}

    Color(uint8_t _r, uint8_t _g, uint8_t _b, std::string _name)
    {
        r = _r;
        g = _g;
        b = _b;
        name = std::move(name);
    }

    uint8_t r;
    uint8_t g;
    uint8_t b;

    std::string name;
};
