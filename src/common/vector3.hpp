#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include "util.hpp"

#include <cmath>
#include <iostream>

class Vector3
{
public:
    double coord[3];
    
    Vector3(): coord{0, 0, 0} {}
    Vector3(double x, double y, double z): coord{x, y, z} {}

    double x() const
    {
        return coord[0];
    }

    double y() const
    {
        return coord[1];
    }

    double z() const
    {
        return coord[2];
    }

    Vector3 operator-() const
    {
        return Vector3(-coord[0], -coord[1], -coord[2]);
    }

    double operator[](int i) const
    {
        return coord[i];
    }

    double& operator[](int i)
    {
        return coord[i];
    }

    Vector3& operator+=(const Vector3& vector)
    {
        coord[0] += vector.coord[0];
        coord[1] += vector.coord[1];
        coord[2] += vector.coord[2];
        return *this;
    }

    Vector3& operator*=(const double scale)
    {
        coord[0] *= scale;
        coord[1] *= scale;
        coord[2] *= scale;
        return *this;
    }

    Vector3& operator/=(const double scale)
    {
        *this *= 1 / scale;
        return *this;
    }

    double length() const
    {
        return std::sqrt(length_squared());
    }

    double length_squared() const
    {
        return coord[0] * coord[0] + coord[1] * coord[1] + coord[2] * coord[2];
    }

    bool near_zero() const
    {
        const auto epsilon = 1e-8;
        return std::fabs(coord[0] < epsilon) && std::fabs(coord[1] < epsilon) && std::fabs(coord[2] < epsilon);
    }

    inline static Vector3 random()
    {
        return Vector3{random_double(), random_double(), random_double()};
    }

    inline static Vector3 random(double min, double max)
    {
        return Vector3{random_double(min, max), random_double(min, max), random_double(min, max)};
    }
};

using Point3 = Vector3;
using Color = Vector3;

inline std::ostream& operator<<(std::ostream& output, const Vector3& vector)
{
    return output << vector.coord[0] << ' ' << vector.coord[1] << ' ' << vector.coord[2];
}

inline std::istream& operator>>(std::istream& input, Vector3& vector)
{
    return input >> vector.coord[0] >> vector.coord[1] >> vector.coord[2];
}

inline Vector3 operator+(const Vector3& u, const Vector3& v)
{
    return Vector3{u.coord[0] + v.coord[0], u.coord[1] + v.coord[1], u.coord[2] + v.coord[2]};
}

inline Vector3 operator-(const Vector3& u, const Vector3& v)
{
    return Vector3{u.coord[0] - v.coord[0], u.coord[1] - v.coord[1], u.coord[2] - v.coord[2]};
}

inline Vector3 operator*(const Vector3& u, const Vector3& v)
{
    return Vector3{u.coord[0] * v.coord[0], u.coord[1] * v.coord[1], u.coord[2] * v.coord[2]};
}

inline Vector3 operator*(double scale, const Vector3& vec)
{
    return Vector3{scale * vec.coord[0], scale * vec.coord[1], scale * vec.coord[2]};
}

inline Vector3 operator*(const Vector3& vec, double scale)
{
    return scale * vec;
}

inline Vector3 operator/(const Vector3& vec, double scale)
{
    return (1 / scale) * vec;
}

inline bool operator==(const Vector3& u, const Vector3& v)
{
    return u.coord[0] == v.coord[0] && u.coord[1] == v.coord[1] && u.coord[2] == v.coord[2];
}

inline bool operator !=(const Vector3& u, const Vector3& v)
{
    return !(u == v);
}

inline double dot(const Vector3& u, const Vector3& v)
{
    return u.coord[0] * v.coord[0] + u.coord[1] * v.coord[1] + u.coord[2] * v.coord[2];
}

inline Vector3 cross(const Vector3& u, const Vector3& v)
{
    return Vector3{u.coord[1] * v.coord[2] - u.coord[2] * v.coord[1],
                    u.coord[2] * v.coord[0] - u.coord[0] * v.coord[2],
                    u.coord[0] * v.coord[1] - u.coord[1] * v.coord[0]};
}

inline Vector3 unit_vector(const Vector3& vec)
{
    return vec / vec.length();
}

inline Vector3 random_in_unit_sphere()
{
    Vector3 random_vec;

    while (true)
    {
        random_vec = Vector3::random(-1.0, 1.0);

        if (random_vec.length_squared() < 1.0)
        {
            break;
        }
    }

    return random_vec;
}

Vector3 random_unit_vector()
{
    return unit_vector(random_in_unit_sphere());
}

Vector3 random_in_unit_disk()
{
    Vector3 random_vec;

    while (true)
    {
        random_vec = Vector3(random_double(-1, 1), random_double(-1, 1), 0);
        
        if (random_vec.length_squared() < 1)
        {
            break;
        }
    }

    return random_vec;
}

Vector3 reflect(const Vector3& v, const Vector3& unit_normal)
{
    return v - 2 * dot(v, unit_normal) * unit_normal;
}

/*
Note:
refractive_indices_ratio is the ratio between the incident index and the refracted index

Supposing that R is the incident ray and N is the normal vector at the incident
point (and also supposing that both R and N are unit vectors), the refracted ray
can be found by the following formulas:
R_perpendicular = ni/nr * (R + cos(theta_incident) * N)
R_parallel = -N * sqrt(1 - |R_perpendicular|^2)

See "Reflections and Refractions in Ray Tracing" by Bram de Greve for more
details and proof.
Tip: to make the proof more explicit, use projections of R on N to find R_perpendicular.
*/
Vector3 refract(const Vector3& incident_ray, const Vector3& normal, double refractive_indices_ratio)
{
    const auto cos_theta = std::fmin(dot(-incident_ray, normal), 1.0);
    Vector3 refracted_perpendicular = refractive_indices_ratio * (incident_ray + cos_theta * normal);
    Vector3 refracted_parallel = -std::sqrt(std::fabs(1.0 - refracted_perpendicular.length_squared())) * normal;
    return refracted_perpendicular + refracted_parallel;
}

#endif // VECTOR3_HPP