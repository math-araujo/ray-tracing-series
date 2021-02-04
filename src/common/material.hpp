#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "color.hpp"
#include "hittable.hpp"
#include "ray.hpp"
#include "util.hpp"

class Material
{
public:
    virtual bool scatter(const Ray& incoming_ray, const HitRecord& record, Color& attenuation, Ray& scattered_ray) const = 0;
};

class Lambertian: public Material
{
public:
    Color albedo; // Measures how much light is reflected without being absorved

    Lambertian(const Color& color): albedo{color} {}
    
    virtual bool scatter(const Ray& incoming_ray, const HitRecord& record, Color& attenuation, Ray& scattered_ray) const override;
};

bool Lambertian::scatter(const Ray& incoming_ray, const HitRecord& record, Color& attenuation, Ray& scattered_ray) const
{
    auto scatter_direction = record.normal + random_unit_vector();

    // Catch degenerate scatter direction
    if (scatter_direction.near_zero())
    {
        scatter_direction = record.normal;
    }

    scattered_ray = Ray{record.point, scatter_direction};
    attenuation = albedo;

    return true;
}

class Metal: public Material
{
public:
    Color albedo;
    double fuzz; 

    Metal(const Color& color, double fuzz_parameter): albedo{color}, fuzz{fuzz_parameter < 1 ? fuzz_parameter : 1} {}

    virtual bool scatter(const Ray& incoming_ray, const HitRecord& record, Color& attenuation, Ray& scattered_ray) const override;
};

bool Metal::scatter(const Ray& incoming_ray, const HitRecord& record, Color& attenuation, Ray& scattered_ray) const
{
    Vector3 reflected = reflect(unit_vector(incoming_ray.direction()), record.normal);
    scattered_ray = Ray{record.point, reflected + fuzz * random_in_unit_sphere()};
    attenuation = albedo;

    return dot(scattered_ray.direction(), record.normal) > 0;
}

#endif // MATERIAL_HPP