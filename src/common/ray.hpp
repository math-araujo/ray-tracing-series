#ifndef RAY_HPP
#define RAY_HPP

#include "vector3.hpp"

class Ray
{
public:
    Point3 orig;
    Vector3 dir;

    Ray() {}
    Ray(const Point3& origin_point, const Vector3& direction_vector): 
        orig{origin_point}, dir{direction_vector}
    {}

    Point3 origin() const 
    {
        return orig;
    }

    Vector3 direction() const
    {
        return dir;
    }
    
    Point3 at(double parameter) const
    {
        return orig + parameter * dir;
    }
};

#endif // RAY_HPP