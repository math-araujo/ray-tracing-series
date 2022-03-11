#include <cmath>
#include <iomanip>
#include <iostream>

#include "util.hpp"

int main()
{
    const int number_of_runs = 100'000'000;
    const int sqrt_number_of_runs = static_cast<int>(std::sqrt(number_of_runs));
    int is_inside_circle = 0;
    int is_inside_circle_stratified = 0;
    for (int i = 0; i < sqrt_number_of_runs; ++i)
    {
        for (int j = 0; j < sqrt_number_of_runs; ++j)
        {
            auto x = random_double(-1, 1);
            auto y = random_double(-1, 1);
            if (x * x + y * y < 1)
            {
                ++is_inside_circle;
            }

            x = 2 * ((i + random_double()) / sqrt_number_of_runs) - 1;
            y = 2 * ((j + random_double()) / sqrt_number_of_runs) - 1;
            if (x * x + y * y < 1)
            {
                ++is_inside_circle_stratified;
            }
        }
    }

    std::cout << std::fixed << std::setprecision(12);
    std::cout << "Regular Estimate of PI = " << 4 * static_cast<double>(is_inside_circle) / number_of_runs << "\n";
    std::cout << "Stratified Estimate of PI = " << 4 * static_cast<double>(is_inside_circle_stratified) / number_of_runs << "\n";
}