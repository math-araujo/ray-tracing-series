#ifndef VECTOR3_HPP
#define VECTOR3_HPP

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

#endif // VECTOR3_HPP