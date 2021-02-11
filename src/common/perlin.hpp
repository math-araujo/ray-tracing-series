#ifndef PERLIN_HPP
#define PERLIN_HPP

#include "vector3.hpp"
#include "util.hpp"
#include <algorithm>
#include <array>

#include <iostream>

class Perlin
{
public:
    Perlin();

    double noise(const Point3& point) const;
    double turbulence(const Point3& point, int depth = 7) const;
private:
    static constexpr int point_count{256};
    std::array<Vector3, point_count> random_vec;
    std::array<int, point_count> permutation_x;
    std::array<int, point_count> permutation_y;
    std::array<int, point_count> permutation_z;

    static std::array<int, point_count> perlin_generate_permutation();
    static void permute(std::array<int, point_count>& array, int size);
    static double perlin_interpolation(Vector3 colors[2][2][2], double u, double v, double w);
};

Perlin::Perlin()
{
    for (int i = 0; i < point_count; ++i)
    {
        random_vec[i] = unit_vector(Vector3::random(-1, 1));
    }

    permutation_x = perlin_generate_permutation();
    permutation_y = perlin_generate_permutation();
    permutation_z = perlin_generate_permutation();
}

std::array<int, Perlin::point_count> Perlin::perlin_generate_permutation()
{
    std::array<int, point_count> array;

    for (int i = 0; i < point_count; ++i)
    {
        array[i] = i;
    }

    permute(array, point_count);

    return array;
}

void Perlin::permute(std::array<int, Perlin::point_count>& array, int size)
{
    for (int i = size - 1; i > 0; --i)
    {
        int target = random_int(0, i);
        std::swap(array[i], array[target]);
    }
}

double Perlin::noise(const Point3& point) const 
{
    auto i = static_cast<int>(std::floor(point.x())) & 255;
    auto j = static_cast<int>(std::floor(point.y())) & 255;
    auto k = static_cast<int>(std::floor(point.z())) & 255;
    Vector3 colors[2][2][2];

    //return random_float[permutation_x[i] ^ permutation_y[j] ^permutation_z[k]];
    for (int delta_i = 0; delta_i < 2; ++delta_i)
    {
        for (int delta_j = 0; delta_j < 2; ++delta_j)
        {
            for (int delta_k = 0; delta_k < 2; ++delta_k)
            {
                auto index = permutation_x[(i + delta_i) & 255] ^ permutation_y[(j + delta_j) & 255] ^ permutation_z[(k + delta_k) & 255];
                colors[delta_i][delta_j][delta_k] = random_vec[index];
            }
        }
    }

    auto u = point.x() - std::floor(point.x());
    auto v = point.y() - std::floor(point.y());
    auto w = point.z() - std::floor(point.z());
    
    return perlin_interpolation(colors, u, v, w);
}

double Perlin::perlin_interpolation(Vector3 colors[2][2][2], double u, double v, double w)
{
    // Hermitian cubic smoothing
    auto uu = u * u * (3 - 2 * u);
    auto vv = v * v * (3 - 2 * v);
    auto ww = w * w * (3 - 2 * w);
    auto accum = 0.0;

    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            for (int k = 0; k < 2; ++k)
            {
                Vector3 weight_v{u - i, v - j, w - k};
                accum += (i * uu + (1 - i) * (1 - uu)) 
                       * (j * vv + (1 - j) * (1 - vv)) 
                       * (k * ww + (1 - k) * (1 - ww))
                       * dot(colors[i][j][k], weight_v);
            }
        }
    }

    return accum;
}

double Perlin::turbulence(const Point3& point, int depth) const
{
    auto accum = 0.0;
    auto temp_point = point;
    auto weight = 1.0;
    
    for (int i = 0; i < depth; ++i)
    {
        accum += weight * noise(temp_point);
        weight *= 0.5;
        temp_point *= 2;
    }

    return std::fabs(accum);
}
#endif // PERLIN_HPP