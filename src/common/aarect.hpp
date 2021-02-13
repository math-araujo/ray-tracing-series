#ifndef AARECT_HPP
#define AARECT_HPP

#include "aabb.hpp"
#include "hittable.hpp"
#include "util.hpp"
#include <memory>

class XYRect: public Hittable
{
public:
    double x0;
    double x1;
    double y0;
    double y1;
    double z_plane_constant;
    std::shared_ptr<Material> material;

    XYRect() {}
    XYRect(double first_x, double second_x, double first_y, double second_y, double z_plane, std::shared_ptr<Material> mat):
           x0{first_x}, x1{second_x}, y0{first_y}, y1{second_y}, z_plane_constant{z_plane}, material{mat} {}

    virtual bool hit(const Ray& ray, double min_parameter, double max_parameter, HitRecord& record) const override;
    virtual bool bounding_box(double start_time, double end_time, AABB& output_box) const override;
};

/*
    For a XY Rectangle lying on the z-plane z = k, for a constant k,
    the intersection of ray R = A + B*t and the rectangle happens for
      
        t = (k - A.z()) / B.z()
    
    if the intersection happens at all. Then it's necessary to compute
    the x and y coordinates and check wheter they are inside the rectangle
    boundaries.
*/
bool XYRect::hit(const Ray& ray, double min_parameter, double max_parameter, HitRecord& record) const
{
    auto intersection_parameter = (z_plane_constant - ray.origin().z()) / ray.direction().z();
    if (intersection_parameter < min_parameter || intersection_parameter > max_parameter)
    {
        return false;
    }

    auto x = ray.origin().x() + intersection_parameter * ray.direction().x();
    auto y = ray.origin().y() + intersection_parameter * ray.direction().y();
    if (x < x0 || x > x1 || y < y0 || y > y1)
    {
        return false;
    }

    record.u = (x - x0) / (x1 - x0);
    record.v = (y - y0) / (y1 - y0);
    record.parameter = intersection_parameter;
    auto outward_normal = Vector3{0, 0, 1};
    record.set_face_normal(ray, outward_normal);
    record.material = material;
    record.point = ray.at(intersection_parameter);

    return true;
}

bool XYRect::bounding_box(double start_time, double end_time, AABB& output_box) const
{
    /*
    The bounding box cannot have a zero width in any dimension, so it's necessary to
    pad the z-axis.
    */
    const auto z_axis_pad = 0.0001;
    output_box = AABB{Point3{x0, y0, z_plane_constant - z_axis_pad}, Point3{x1, y1, z_plane_constant + z_axis_pad}};
    return true;
}

class XZRect: public Hittable
{
public:
    double x0;
    double x1;
    double z0;
    double z1;
    double y_plane_constant;
    std::shared_ptr<Material> material;

    XZRect() {}
    XZRect(double first_x, double second_x, double first_z, double second_z, double y_plane, std::shared_ptr<Material> mat):
           x0{first_x}, x1{second_x}, z0{first_z}, z1{second_z}, y_plane_constant{y_plane}, material{mat} {}
    
    virtual bool hit(const Ray& ray, double min_parameter, double max_parameter, HitRecord& record) const override;
    virtual bool bounding_box(double start_time, double end_time, AABB& output_box) const override;
};

bool XZRect::hit(const Ray& ray, double min_parameter, double max_parameter, HitRecord& record) const 
{
    auto intersection_parameter = (y_plane_constant - ray.origin().y()) / ray.direction().y();
    if (intersection_parameter < min_parameter || intersection_parameter > max_parameter)
    {
        return false;
    }

    auto x = ray.origin().x() + intersection_parameter * ray.direction().x();
    auto z = ray.origin().z() + intersection_parameter * ray.direction().z();
    if (x < x0 || x > x1 || z < z0 || z > z1)
    {
        return false;
    }

    record.u = (x - x0) / (x1 - x0);
    record.v = (z - z0) / (z1 - z0);
    record.parameter = intersection_parameter;
    auto outward_normal = Vector3{0, 1, 0};
    record.set_face_normal(ray, outward_normal);
    record.material = material;
    record.point = ray.at(intersection_parameter);

    return true;
}

bool XZRect::bounding_box(double start_time, double end_time, AABB& output_box) const 
{
    /*
    The bounding box cannot have a zero width in any dimension, so it's necessary to
    pad the y-axis.
    */
    const auto y_axis_pad = 0.0001;
    output_box = AABB{Point3{x0, y_plane_constant - y_axis_pad, z0}, Point3{x1, y_plane_constant + y_axis_pad, z1}};
    return true;
}

class YZRect: public Hittable
{
public:
    double y0;
    double y1;
    double z0;
    double z1;
    double x_plane_constant;
    std::shared_ptr<Material> material;

    YZRect() {}
    YZRect(double first_y, double second_y, double first_z, double second_z, double x_plane, std::shared_ptr<Material> mat):
           y0{first_y}, y1{second_y}, z0{first_z}, z1{second_z}, x_plane_constant{x_plane}, material{mat} {}
    
    virtual bool hit(const Ray& ray, double min_parameter, double max_parameter, HitRecord& record) const override;
    virtual bool bounding_box(double start_time, double end_time, AABB& output_box) const override;
};

bool YZRect::hit(const Ray& ray, double min_parameter, double max_parameter, HitRecord& record) const
{
    auto intersection_parameter = (x_plane_constant - ray.origin().x()) / ray.direction().x();
    if (intersection_parameter < min_parameter || intersection_parameter > max_parameter)
    {
        return false;
    }

    auto y = ray.origin().y() + intersection_parameter * ray.direction().y();
    auto z = ray.origin().z() + intersection_parameter * ray.direction().z();
    if (y < y0 || y > y1 || z < z0 || z > z1)
    {
        return false;
    }

    record.u = (y - y0) / (y1 - y0);
    record.v = (z - z0) / (z1 - z0);
    record.parameter = intersection_parameter;
    auto outward_normal = Vector3{1, 0, 0};
    record.set_face_normal(ray, outward_normal);
    record.material = material;
    record.point = ray.at(intersection_parameter);

    return true;
}

bool YZRect::bounding_box(double start_time, double end_time, AABB& output_box) const 
{
    /*
    The bounding box cannot have a zero width in any dimension, so it's necessary to
    pad the x-axis.
    */
    const auto x_axis_pad = 0.0001;
    output_box = AABB{Point3{x_plane_constant - x_axis_pad, y0, z0}, Point3{x_plane_constant + x_axis_pad, y1, z1}};
    return true;
}

#endif // AARECT_HPP