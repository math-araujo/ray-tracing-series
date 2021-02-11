#ifndef AABB_HPP
#define AABB_HPP

#include "ray.hpp"
#include "vector3.hpp"

#include <cmath>

class AABB
{
public:
    Point3 minimum;
    Point3 maximum;

    AABB() {}
    AABB(const Point3& min, const Point3& max): minimum{min}, maximum{max} {}

    Point3 min() const;
    Point3 max() const;
    bool hit(const Ray& ray, double left_end, double right_end) const;
};

Point3 AABB::min() const
{
    return minimum;
}

Point3 AABB::max() const
{
    return maximum;
}

bool AABB::hit(const Ray& ray, double left_end, double right_end) const
{
    for (int i = 0; i < 3; ++i)
    {
        auto inverse_direction = 1.0f / ray.direction()[i];
        auto temp_left_end = (min()[i] - ray.origin()[i]) * inverse_direction;
        auto temp_right_end = (max()[i] - ray.origin()[i]) * inverse_direction;

        if (inverse_direction < 0.0f)
        {
            std::swap(temp_left_end, temp_right_end);
        }

        left_end = temp_left_end > left_end ? temp_left_end : left_end;
        right_end = temp_right_end < right_end ? temp_right_end : right_end;

        if (right_end <= left_end)
        {
            return false;
        }
    }

    return true;
}

AABB surrounding_box(AABB& box0, AABB box1)
{
    Point3 minimum{std::fmin(box0.min().x(), box1.min().x()), 
                   std::fmin(box0.min().y(), box1.min().y()),
                   std::fmin(box0.min().z(), box1.min().z())};
    
    Point3 maximum{std::fmax(box0.max().x(), box1.max().x()),
                   std::fmax(box0.max().y(), box1.max().y()), 
                   std::fmax(box0.max().z(), box1.max().z())};
    
    return AABB{minimum, maximum};
}

#endif // AABB_HPP