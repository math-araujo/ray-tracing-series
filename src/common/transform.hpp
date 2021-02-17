#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "aabb.hpp"
#include "hittable.hpp"
#include "ray.hpp"
#include "util.hpp"
#include "vector3.hpp"
#include <memory>

class Translate: public Hittable
{
public:
    std::shared_ptr<Hittable> instance; // pointer to instance to be translated
    Vector3 offset;

    Translate(std::shared_ptr<Hittable> object, const Vector3& displacement): instance{object}, offset{displacement} {}

    virtual bool hit(const Ray& ray, double min_parameter, double max_parameter, HitRecord& record) const override;
    virtual bool bounding_box(double start_time, double end_time, AABB& output_box) const override;
};

bool Translate::hit(const Ray& ray, double min_parameter, double max_parameter, HitRecord& record) const
{
    Ray moved_ray{ray.origin() - offset, ray.direction(), ray.time()};

    if (!instance->hit(moved_ray, min_parameter, max_parameter, record))
    {
        return false;
    }

    record.point += offset;
    record.set_face_normal(moved_ray, record.normal);

    return true;
}

bool Translate::bounding_box(double start_time, double end_time, AABB& output_box) const
{
    if (!instance->bounding_box(start_time, end_time, output_box))
    {
        return false;
    }

    output_box = AABB{output_box.min() + offset, output_box.max() + offset};
    return true;
}

class RotateY: public Hittable
{
public:
    std::shared_ptr<Hittable> instance;
    double sin_theta;
    double cos_theta;
    bool has_box;
    AABB box;

    RotateY(std::shared_ptr<Hittable> object, double angle);

    virtual bool hit(const Ray& ray, double min_parameter, double max_parameter, HitRecord& record) const override;
    virtual bool bounding_box(double start_time, double end_time, AABB& output_box) const override;
};

RotateY::RotateY(std::shared_ptr<Hittable> object, double angle): instance{object}
{
    auto radians = degrees_to_radians(angle);
    sin_theta = std::sin(radians);
    cos_theta = std::cos(radians);
    has_box = instance->bounding_box(0, 1, box);

    Point3 min{infinity, infinity, infinity};
    Point3 max{-infinity, -infinity, -infinity};

    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            for (int k = 0; k < 2; ++k)
            {
                auto x = i * box.max().x() + (1 - i) * box.min().x();
                auto y = j * box.max().y() + (1 - j) * box.min().y();
                auto z = k * box.max().z() + (1 - k) * box.min().z();

                auto rotate_x = cos_theta * x + sin_theta * z;
                auto rotate_z = -sin_theta * x + cos_theta * z;

                Vector3 test{rotate_x, y, rotate_z};
                for (int axis = 0; axis < 3; ++axis)
                {
                    min[axis] = std::fmin(min[axis], test[axis]);
                    max[axis] = std::fmax(max[axis], test[axis]);
                }
            }
        }
    }

    box = AABB{min, max};
}

bool RotateY::hit(const Ray& ray, double min_parameter, double max_parameter, HitRecord& record) const
{
    auto origin = ray.origin();
    auto direction = ray.direction();

    origin[0] = cos_theta * ray.origin()[0] - sin_theta * ray.origin()[2];
    origin[2] = sin_theta * ray.origin()[0] + cos_theta * ray.origin()[2];

    direction[0] = cos_theta * ray.direction()[0] - sin_theta * ray.direction()[2];
    direction[2] = sin_theta * ray.direction()[0] + cos_theta * ray.direction()[2];

    Ray rotated_ray{origin, direction, ray.time()};

    if (!instance->hit(rotated_ray, min_parameter, max_parameter, record))
    {
        return false;
    }

    auto hit_point = record.point;
    auto normal = record.normal;

    hit_point[0] = cos_theta * record.point[0] + sin_theta * record.point[2];
    hit_point[2] = -sin_theta * record.point[0] + cos_theta * record.point[2];

    normal[0] = cos_theta * record.normal[0] + sin_theta * record.normal[2];
    normal[2] = -sin_theta * record.normal[0] + cos_theta * record.normal[2];

    record.point = hit_point;
    record.set_face_normal(rotated_ray, normal);

    return true;
}

bool RotateY::bounding_box(double start_time, double end_time, AABB& output_box) const
{
    output_box = box;
    return has_box;
}

#endif // TRANSFORM_HPP