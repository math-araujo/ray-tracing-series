#ifndef UTIL_HPP
#define UTIL_HPP

#include <cmath>
#include <limits>
#include <memory>
#include <random>

constexpr double infinity = std::numeric_limits<double>::infinity();
constexpr double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees)
{
    return degrees * (pi / 180.0);
}

inline double random_double()
{
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

// Returns a random double in range [min; max[
inline double random_double(double min, double max)
{
    return min + (max - min) * random_double();
}

// Returns a random integer in range [min; max]
inline int random_int(int min, int max)
{
    return static_cast<int>(random_double(min, max + 1));
}

inline double clamp(double value, double min, double max)
{
    if (value < min)
    {
        return min;
    }
    
    if (value > max)
    {
        return max;
    }

    return value;
}

#endif // UTIL_HPP