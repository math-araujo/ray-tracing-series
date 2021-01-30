#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include "ray.hpp"

struct HitRecord
{
    Point3 point;
    Vector3 normal;
    double parameter;
};

class Hittable
{
public:
    virtual bool hit(const Ray& ray, double min_parameter, double max_parameter, HitRecord& record) const = 0;
};

#endif // HITTABLE_HPP