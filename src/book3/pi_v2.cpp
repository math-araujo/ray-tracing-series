#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <limits>

#include "util.hpp"

int main()
{
    std::uint64_t is_inside_circle = 0;
    std::cout << std::fixed << std::setprecision(12);
    std::uint64_t number_of_runs = 1;
    for ( ; number_of_runs < std::numeric_limits<std::uint64_t>::max(); ++number_of_runs)
    {
        const auto x = random_double(-1, 1);
        const auto y = random_double(-1, 1);
        if (x * x + y * y < 1)
        {
            ++is_inside_circle;
        }

        if (number_of_runs % 1'000'000 == 0)
        {
            std::cout << "Estimate of PI = " << 4 * static_cast<double>(is_inside_circle) / number_of_runs << "\n";
        }
    }

    std::cout << "Last estimate of PI = " << 4 * static_cast<double>(is_inside_circle) / number_of_runs << "\n";
}