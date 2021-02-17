#ifndef BOX_HPP
#define BOX_HPP

#include "aarect.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "util.hpp"
#include "vector3.hpp"
#include <memory>

class Box: public Hittable
{
public:
    Point3 box_min;
    Point3 box_max;
    HittableList sides;

    Box() {}
    Box(const Point3& point0, const Point3& point1, std::shared_ptr<Material> material);

    virtual bool hit(const Ray& ray, double min_parameter, double max_parameter, HitRecord& record) const override;
    virtual bool bounding_box(double start_time, double end_time, AABB& output_box) const override;
};

Box::Box(const Point3& point0, const Point3& point1, std::shared_ptr<Material> material): box_min{point0}, box_max{point1}
{
    sides.add(std::make_shared<XYRect>(box_min.x(), box_max.x(), box_min.y(), box_max.y(), box_max.z(), material));
    sides.add(std::make_shared<XYRect>(box_min.x(), box_max.x(), box_min.y(), box_max.y(), box_min.z(), material));

    sides.add(std::make_shared<XZRect>(box_min.x(), box_max.x(), box_min.z(), box_max.z(), box_max.y(), material));
    sides.add(std::make_shared<XZRect>(box_min.x(), box_max.x(), box_min.z(), box_max.z(), box_min.y(), material));

    sides.add(std::make_shared<YZRect>(box_min.y(), box_max.y(), box_min.z(), box_max.z(), box_max.x(), material));
    sides.add(std::make_shared<YZRect>(box_min.y(), box_max.y(), box_min.z(), box_max.z(), box_min.x(), material));
}

bool Box::hit(const Ray& ray, double min_parameter, double max_parameter, HitRecord& record) const
{
    return sides.hit(ray, min_parameter, max_parameter, record);
}

bool Box::bounding_box(double start_time, double end_time, AABB& output_box) const
{
    output_box = AABB{box_min, box_max};
    return true;
}

#endif // BOX_HPP