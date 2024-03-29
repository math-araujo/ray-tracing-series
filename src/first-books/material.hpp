#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "color.hpp"
#include "hittable.hpp"
#include "ray.hpp"
#include "texture.hpp"
#include "util.hpp"
#include <memory>

class Material
{
public:
    virtual bool scatter(const Ray& incoming_ray, const HitRecord& record, Color& attenuation, Ray& scattered_ray) const = 0;
    virtual Color emitted(double u, double v, const Point3& point) const;
};

Color Material::emitted(double u, double v, const Point3& point) const
{
    return Color{0, 0, 0};
}

class Lambertian: public Material
{
public:
    std::shared_ptr<Texture> albedo; // Measures how much light is reflected without being absorved

    Lambertian(const Color& color): albedo{std::make_shared<SolidColor>(color)} {}
    Lambertian(std::shared_ptr<Texture> texture): albedo{texture} {}

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

    scattered_ray = Ray{record.point, scatter_direction, incoming_ray.time()};
    attenuation = albedo->value(record.u, record.v, record.point);

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
    scattered_ray = Ray{record.point, reflected + fuzz * random_in_unit_sphere(), incoming_ray.time()};
    attenuation = albedo;

    return dot(scattered_ray.direction(), record.normal) > 0;
}

class Dielectric: public Material
{
public:
    double index_of_refraction;
    Dielectric(double ir): index_of_refraction{ir} {}

    virtual bool scatter(const Ray& incoming_ray, const HitRecord& record, Color& attenuation, Ray& scattered_ray) const override;
private:    
    // Schlick's approximation for reflectance on dielectrics
    static double reflectance(double cosine, double refraction_ratio);
};

bool Dielectric::scatter(const Ray& incoming_ray, const HitRecord& record, Color& attenuation, Ray& scattered_ray) const
{
    attenuation = Color{1.0, 1.0, 1.0}; // No absorption
    
    /*
    If record.front_face is true, the ray hitted the external surface of the sphere; supposing the
    air as standard medium for light propagation, it's index of refraction is approximately 1.0,
    so the refraction_ratio between the incident medium and material is 1.0 / index_of_refraction.
    */
    double refraction_ratio = record.front_face ? (1.0 / index_of_refraction) : index_of_refraction;
    
    Vector3 unit_direction = unit_vector(incoming_ray.direction());
    double cos_theta = std::fmin(dot(-unit_direction, record.normal), 1.0);
    double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);
    
    bool cannot_refract = refraction_ratio * sin_theta > 1.0; // total reflection
    Vector3 direction;
    
    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
    {
        direction = reflect(unit_direction, record.normal);
    }
    else
    {
        direction = refract(unit_direction, record.normal, refraction_ratio);
    }

    scattered_ray = Ray{record.point, direction, incoming_ray.time()};
    return true;
}

double Dielectric::reflectance(double cosine, double refraction_ratio)
{
    auto r0 = (1 - refraction_ratio) / (1 + refraction_ratio);
    r0 = r0 * r0;
    return r0 + (1 - r0) * std::pow((1 - cosine), 5);
}

class DiffuseLight: public Material
{
public:
    std::shared_ptr<Texture> emit;

    DiffuseLight(std::shared_ptr<Texture> texture): emit{texture} {}
    DiffuseLight(Color color): emit{std::make_shared<SolidColor>(color)} {}

    virtual bool scatter(const Ray& ray, const HitRecord& record, Color& attenuation, Ray& scattered_ray) const override;
    virtual Color emitted(double u, double v, const Point3& point) const override;
};

bool DiffuseLight::scatter(const Ray& incoming_ray, const HitRecord& record, Color& attenuation, Ray& scattered_ray) const
{
    return false; // DiffuseLight don't scatter incoming rays, only emit light
}

Color DiffuseLight::emitted(double u, double v, const Point3& point) const
{
    return emit->value(u, v, point);
}

class Isotropic: public Material
{
public:
    std::shared_ptr<Texture> albedo;

    Isotropic(Color color): albedo{std::make_shared<SolidColor>(color)} {}
    Isotropic(std::shared_ptr<Texture> texture): albedo{texture} {}

    virtual bool scatter(const Ray& incoming_ray, const HitRecord& record, Color& attenuation, Ray& scattered_ray) const override;
};

bool Isotropic::scatter(const Ray& incoming_ray, const HitRecord& record, Color& attenuation, Ray& scattered_ray) const
{
    scattered_ray = Ray{record.point, random_in_unit_sphere(), incoming_ray.time()};
    attenuation = albedo->value(record.u, record.v, record.point);
    return true;
}

#endif // MATERIAL_HPP