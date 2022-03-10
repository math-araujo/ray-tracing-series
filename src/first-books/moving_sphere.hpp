#ifndef MOVING_SPHERE_HPP
#define MOVING_SPHERE_HPP

#include "aabb.hpp"
#include "hittable.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "vector3.hpp"

#include <memory>

class MovingSphere: public Hittable
{
public:
    Point3 begin_center;
    Point3 end_center;
    double begin_time;
    double end_time;
    double radius;
    std::shared_ptr<Material> material;

    MovingSphere(){}
    MovingSphere(Point3 center0, Point3 center1, double time0, double time1, double r, std::shared_ptr<Material> m):
    begin_center{center0}, end_center{center1}, begin_time{time0}, end_time{time1}, radius{r}, material{m}
    {}

    virtual bool hit(const Ray& ray, double min_parameter, double max_parameter, HitRecord& record) const override;
    virtual bool bounding_box(double start_time, double end_time, AABB& output_box) const override;
    Point3 center(double time) const;
};

bool MovingSphere::hit(const Ray& ray, double min_parameter, double max_parameter, HitRecord& record) const
{
    Vector3 center_to_origin = ray.origin() - center(ray.time()); // A - C in the equation
    auto quadratic_coefficient = ray.direction().length_squared();
    auto half_linear_coefficient = dot(ray.direction(), center_to_origin);
    auto constant_coefficient = center_to_origin.length_squared() - radius * radius;

    auto discriminant = half_linear_coefficient * half_linear_coefficient - quadratic_coefficient * constant_coefficient;
    if (discriminant < 0.0)
    {
        return false;
    }

    auto sqrt_discriminant = std::sqrt(discriminant);
    auto root = (-half_linear_coefficient - sqrt_discriminant) / quadratic_coefficient;
    if (root < min_parameter || root > max_parameter)
    {
        root = (-half_linear_coefficient + sqrt_discriminant) / quadratic_coefficient;

        if (root < min_parameter || root > max_parameter)
        {
            return false;
        }
    }

    record.parameter = root;
    record.point = ray.at(root);
    Vector3 outward_normal = (record.point - center(ray.time())) / radius; // Unit length normal
    record.set_face_normal(ray, outward_normal);
    record.material = material;
    
    return true;
}

bool MovingSphere::bounding_box(double start_time, double end_time, AABB& output_box) const
{
    AABB start_box{center(start_time) - Vector3{radius, radius, radius}, 
                   center(start_time) + Vector3{radius, radius, radius}};
    AABB end_box{center(end_time) - Vector3{radius, radius, radius},
                 center(end_time) + Vector3{radius, radius, radius}};
    
    output_box = surrounding_box(start_box, end_box);
    return true;
}

Point3 MovingSphere::center(double time) const
{
    return begin_center + ((time - begin_time) / (end_time - begin_time)) * (end_center - begin_center);
}

#endif // MOVING_SPHERE_HPP