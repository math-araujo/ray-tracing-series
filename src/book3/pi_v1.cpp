#include <cmath>
#include <iomanip>
#include <iostream>

#include "util.hpp"

int main()
{
    const int number_of_runs = 1000;
    int is_inside_circle = 0;
    for (int i = 0; i < number_of_runs; ++i)
    {
        const auto x = random_double(-1, 1);
        const auto y = random_double(-1, 1);
        if (x * x + y * y < 1)
        {
            ++is_inside_circle;
        }
    }

    std::cout << std::fixed << std::setprecision(12);
    std::cout << "Estimate of PI = " << 4 * static_cast<double>(is_inside_circle) / number_of_runs << "\n";
}