#ifndef RAY_HPP
#define RAY_HPP

#include "vector3.hpp"

class Ray
{
public:
    Point3 orig;
    Vector3 dir;
    double tm;

    Ray() {}
    Ray(const Point3& origin_point, const Vector3& direction_vector, double time): 
        orig{origin_point}, dir{direction_vector}, tm{time}
    {}

    Point3 origin() const 
    {
        return orig;
    }

    Vector3 direction() const
    {
        return dir;
    }
    
    double time() const 
    {
        return tm;
    }
    
    Point3 at(double parameter) const
    {
        return orig + parameter * dir;
    }
};

#endif // RAY_HPP