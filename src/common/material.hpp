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

class Dielectric: public Material
{
public:
    double index_of_refraction;
    Dielectric(double ir): index_of_refraction{ir} {}

    virtual bool scatter(const Ray& incoming_ray, const HitRecord& record, Color& attenuation, Ray& scattered_ray) const override;
};

bool Dielectric::scatter(const Ray& incoming_ray, const HitRecord& record, Color& attenuation, Ray& scattered_ray) const
{
    attenuation = Color{1.0, 1.0, 1.0};
    
    /*
    If record.front_face is true, the ray hitted the external surface of the sphere; supposing the
    air as standard medium for light propagation, it's index of refraction is approximately 1.0,
    so the refraction_ratio between the incident medium and material is 1.0 / index_of_refraction.
    */
    double refraction_ratio = record.front_face ? (1.0 / index_of_refraction) : index_of_refraction;
    
    Vector3 unit_direction = unit_vector(incoming_ray.direction());
    Vector3 refracted_direction = refract(unit_direction, record.normal, refraction_ratio);

    scattered_ray = Ray{record.point, refracted_direction};
    return true;
}

#endif // MATERIAL_HPP