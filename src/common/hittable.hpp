#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include "ray.hpp"

struct HitRecord
{
    Point3 point;
    Vector3 normal;
    double parameter;
    bool front_face; // stores whether the ray is outside the sphere or not

    inline void set_face_normal(const Ray& ray, const Vector3& outward_normal)
    {
        front_face = dot(ray.direction(), outward_normal) < 0.0;
        normal = front_face ? outward_normal : outward_normal;
    }
};

class Hittable
{
public:
    virtual bool hit(const Ray& ray, double min_parameter, double max_parameter, HitRecord& record) const = 0;
};

#endif // HITTABLE_HPP