#ifndef COLOR_HPP
#define COLOR_HPP

#include "util.hpp"
#include "vector3.hpp"

#include <iostream>

void write_color(std::ostream& out, Color pixel_color, int samples_per_pixel)
{
    auto red = pixel_color.x();
    auto green = pixel_color.y();
    auto blue = pixel_color.z();

    auto scale = 1.0 / samples_per_pixel;
    red *= scale;
    green *= scale;
    blue *= scale;
    
    out << static_cast<int>(256 * clamp(red, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(green, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(blue, 0.0, 0.999)) << '\n';
}

#endif // COLOR_HPP