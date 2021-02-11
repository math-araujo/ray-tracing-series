#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "hittable.hpp"
#include "ray.hpp"
#include "util.hpp"
#include <cmath>
#include <memory>

class Sphere: public Hittable
{
public:
    Point3 center;
    double radius;
    std::shared_ptr<Material> material;

    Sphere() {}
    Sphere(Point3 cen, double r, std::shared_ptr<Material> m): center{cen}, radius{r}, material{m} {}

    virtual bool hit(const Ray& ray, double min_parameter, double max_parameter, HitRecord& record) const override;
    virtual bool bounding_box(double start_time, double end_time, AABB& output_box) const override;
private:
    static void get_sphere_uv(const Point3& point, double& u, double& v);
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
    Vector3 center_to_origin = ray.origin() - center; // A - C in the equation
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
    Vector3 outward_normal = (record.point - center) / radius; // Unit length normal
    record.set_face_normal(ray, outward_normal);
    get_sphere_uv(outward_normal, record.u, record.v);
    record.material = material;
    
    return true;
}

bool Sphere::bounding_box(double start_time, double end_time, AABB& output_box) const 
{
    output_box = AABB{center - Vector3{radius, radius, radius}, center + Vector3{radius, radius, radius}};
    return true;
}

/* 
    Converts a point (x, y, z) on the unit sphere to UV coordinates (u, v)

    The mapping is such that 
        u = phi / (2 * PI)
        v = theta / PI
    where phi is the angle around the Y-axis when it's pointing-up (i.e. longitude)
    and theta is the angle from -Y to Y (i.e. latitude)

    phi and theta can be found converting the cartesian coordinates (x, y, z) to
    spherical coordinates:
        y = - cos(theta)
        x = - cos(phi) * sin(theta)
        z =   sin(phi) * sin(theta)

    Args:
        point: point on the unit sphere
        u: UV coordinate in range [0; 1], mapped to phi (longitude)
        v: UV coordinate in range [0; 1], mapped to theta (latitude)
*/
void Sphere::get_sphere_uv(const Point3& point, double& u, double& v)
{
    auto phi = std::atan2(-point.z(), point.x()) + pi;
    auto theta = std::acos(-point.y());

    u = phi / (2* pi);
    v = theta / pi;
}

#endif // SPHERE_HPP