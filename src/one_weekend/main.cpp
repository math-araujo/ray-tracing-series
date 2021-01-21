#include "color.hpp"
#include "vector3.hpp"

#include <iostream>

int main()
{
    // Image settings
    constexpr int image_width = 256;
    constexpr int image_height = 256;

    // Render
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
    
    // Starting at upper left corner, left to right and up to bottom
    for (int row = image_height - 1; row >= 0; --row)
    {
        std::cerr << "\rScanlines remaining: " << row << ' ' << std::flush;

        for (int column = 0; column < image_width; ++column)
        {
            auto red = double(column) / (image_width - 1);
            auto green = double(row) / (image_height - 1);
            auto blue = 0.25;

            write_color(std::cout, Color{red, green, blue});
        }
    }
    
    std::cerr << "\nDone.\n";
}