#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "ray.hpp"
#include "util.hpp"
#include <cmath>

class Camera
{
public:
    Camera(Point3 look_from, Point3 look_at, Vector3 view_up, double vertical_fov, double aspect_ratio);

    Ray get_ray(double u, double v) const;
private:
    Point3 origin{0, 0, 0};
    Point3 lower_left_corner;
    Vector3 horizontal;
    Vector3 vertical;
};

Camera::Camera(Point3 look_from, Point3 look_at, Vector3 view_up, double vertical_fov, double aspect_ratio): 
    origin{look_from}
{
    const auto theta = degrees_to_radians(vertical_fov);
    const auto height = std::tan(theta / 2);
    const auto viewport_height = 2.0 * height;
    const auto viewport_width = aspect_ratio * viewport_height; // 3.555...

    // Basis vectors for the camera frame of reference
    auto w = unit_vector(look_from - look_at); // Vector on opposite direction to the gaze direction
    auto u = unit_vector(cross(view_up, w));
    auto v = cross(w, u);

    horizontal = viewport_width * u;
    vertical = viewport_height * v;
    lower_left_corner = origin - horizontal / 2 - vertical / 2 - w;
}

Ray Camera::get_ray(double s, double t) const
{
    return Ray{origin, lower_left_corner + s * horizontal + t * vertical - origin};
}

#endif // CAMERA_HPP