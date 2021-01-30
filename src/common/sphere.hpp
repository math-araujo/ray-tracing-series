#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "hittable.hpp"
#include "ray.hpp"

class Sphere: public Hittable
{
public:
    Point3 center;
    double radius;

    Sphere() {}
    Sphere(Point3 cen, double r): center{cen}, radius{r} {}

    virtual bool hit(const Ray& ray, double min_parameter, double max_parameter, HitRecord& record) const override;
};

/*
    Ray-Sphere Intersection:
        t^2 * dot(b, b) + 2 * t * dot(b, A - C) + dot(A - C, A - C) - radius^2 = 0
        where b is ray.direction(), A is ray.origin(), C is the center of the sphere

        Note that dot(b, b) == ||b||^2
    
    Return true if the ray hits the sphere, false otherwise.
    
    If the ray hitted the sphere, the intersection point, the surface normal at the
    intersection point and the root/parameter are stored in HitRecord.
*/
bool Sphere::hit(const Ray& ray, double min_parameter, double max_parameter, HitRecord& record) const
{
    Vector3 center_to_origin = ray.origin() - center;
    auto quadratic_coefficient = ray.direction().length_squared();
    auto half_linear_coefficient = dot(ray.direction(), center_to_origin);
    auto constant_coefficient = center_to_origin.length_squared() - radius * radius;

    auto discriminant = half_linear_coefficient * half_linear_coefficient - quadratic_coefficient * constant_coefficient;
    if (discriminant < 0.0)
    {
        return false;
    }

    auto sqrt_discriminant = std::sqrt(discriminant);
    auto root = (half_linear_coefficient - sqrt_discriminant) / quadratic_coefficient;
    if (root < min_parameter || root > max_parameter)
    {
        root = (half_linear_coefficient + sqrt_discriminant) / quadratic_coefficient;

        if (root < min_parameter || root > max_parameter)
        {
            return false;
        }
    }

    record.parameter = root;
    record.point = ray.at(root);
    Vector3 outward_normal = (record.point - center) / radius; // Unit length normal
    record.set_face_normal(ray, outward_normal);

    return true;
}

#endif // SPHERE_HPP