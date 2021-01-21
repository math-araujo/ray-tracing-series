#ifndef COLOR_HPP
#define COLOR_HPP

#include "vector3.hpp"

#include <iostream>

void write_color(std::ostream& out, Color pixel_color)
{
    out << static_cast<int>(255.999 * pixel_color.x()) << ' '
        << static_cast<int>(255.999 * pixel_color.y()) << ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

#endif // COLOR_HPP