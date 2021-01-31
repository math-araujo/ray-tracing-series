#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "ray.hpp"
#include "util.hpp"

class Camera
{
public:
    Camera();

    Ray get_ray(double u, double v) const;
private:
    Point3 origin{0, 0, 0};
    Point3 lower_left_corner;
    Vector3 horizontal;
    Vector3 vertical;
};

Camera::Camera()
{
    const auto aspect_ratio = 16.0 / 9.0;
    const auto viewport_height = 2.0;
    const auto viewport_width = aspect_ratio * viewport_height; // 3.555...
    const auto focal_length = 1.0;

    horizontal = Vector3{viewport_width, 0.0, 0.0};
    vertical = Vector3{0.0, viewport_height, 0.0};
    lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vector3{0.0, 0.0, focal_length};
}

Ray Camera::get_ray(double u, double v) const
{
    return Ray{origin, lower_left_corner + u * horizontal + v * vertical - origin};
}

#endif // CAMERA_HPP