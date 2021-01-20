#include <iostream>

int main()
{
    // Image settings
    constexpr int image_width = 256;
    constexpr int image_height = 256;
    constexpr double max_color_intensity = 255.999;

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

            int int_red = static_cast<int>(max_color_intensity * red);
            int int_green = static_cast<int>(max_color_intensity * green);
            int int_blue = static_cast<int>(max_color_intensity * blue);

            std::cout << int_red << ' ' << int_green << ' ' << int_blue << '\n';
        }
    }
    
    std::cerr << "\nDone.\n";
}