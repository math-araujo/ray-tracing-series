#ifndef CONSTANT_MEDIUM_HPP
#define CONSTANT_MEDIUM_HPP

#include "hittable.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "texture.hpp"
#include "vector3.hpp"

#include <memory>

class ConstantMedium: public Hittable
{
public:
    std::shared_ptr<Hittable> boundary;
    double negative_inverse_density;
    std::shared_ptr<Material> phase_function;

    ConstantMedium(std::shared_ptr<Hittable> medium_boundary, double density, std::shared_ptr<Texture> texture):
                   boundary{medium_boundary}, negative_inverse_density{-1 / density}, phase_function{std::make_shared<Isotropic>(texture)} {}
    ConstantMedium(std::shared_ptr<Hittable> medium_boundary, double density, Color color):
                   boundary{medium_boundary}, negative_inverse_density{-1 / density}, phase_function{std::make_shared<Isotropic>(color)} {}

    virtual bool hit(const Ray& ray, double min_parameter, double max_parameter, HitRecord& record) const override;
    virtual bool bounding_box(double start_time, double end_time, AABB& output_box) const override;
};

bool ConstantMedium::hit(const Ray& ray, double min_parameter, double max_parameter, HitRecord& record) const 
{
    HitRecord min_hit;
    HitRecord max_hit;

    if (!boundary->hit(ray, -infinity, infinity, min_hit))
    {
        return false;
    }

    if (!boundary->hit(ray, min_hit.parameter + 0.0001, infinity, max_hit))
    {
        return false;
    }

    min_hit.parameter = std::max(min_parameter, min_hit.parameter);
    max_hit.parameter = std::min(max_parameter, max_hit.parameter);

    if (min_hit.parameter >= max_hit.parameter)
    {
        return false;
    }

    min_hit.parameter = std::max(0.0, min_hit.parameter);

    /*
    Derivation for the hit_distance can be found here:
    http://psgraphics.blogspot.com/2013/11/scattering-in-constant-medium.html
    */
    const auto ray_length = ray.direction().length();
    const auto distance_inside_boundary = (max_hit.parameter - min_hit.parameter) * ray_length;
    const auto hit_distance = negative_inverse_density * log(random_double()); 

    if (hit_distance > distance_inside_boundary)
    {
        return false;
    }

    record.parameter = min_hit.parameter + hit_distance / ray_length;
    record.point = ray.at(record.parameter);

    record.normal = Vector3{1, 0, 0}; // arbitrary
    record.front_face = true; // arbitrary
    record.material = phase_function;

    return true;
}

bool ConstantMedium::bounding_box(double start_time, double end_time, AABB& output_box) const
{
    return boundary->bounding_box(start_time, end_time, output_box);
}

#endif // CONSTANT_MEDIUM_HPP