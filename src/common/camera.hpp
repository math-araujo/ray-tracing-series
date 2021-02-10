#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "ray.hpp"
#include "util.hpp"
#include <cmath>

class Camera
{
public:
    // By default, the Camera has no depth of field/defocus blue
    Camera(Point3 look_from, Point3 look_at, Vector3 view_up, double vertical_fov, double aspect_ratio, 
        double aperture = 0.0, double focus_distance = 1.0, double time0 = 0.0, double time1 = 0.0);

    Ray get_ray(double u, double v) const;
private:
    Point3 origin{0, 0, 0};
    Point3 lower_left_corner;
    Vector3 horizontal;
    Vector3 vertical;
    // Basis vectors of the camera frame of reference
    Vector3 u, v, w;
    double lens_radius;
    // Open/close times of the shutter
    double open_time;
    double close_time;
};

Camera::Camera(Point3 look_from, Point3 look_at, Vector3 view_up, double vertical_fov, double aspect_ratio, 
    double aperture, double focus_distance, double time0, double time1):
    origin{look_from}, lens_radius{aperture / 2}, open_time{time0}, close_time{time1}
{
    const auto theta = degrees_to_radians(vertical_fov);
    const auto height = std::tan(theta / 2);
    const auto viewport_height = 2.0 * height;
    const auto viewport_width = aspect_ratio * viewport_height; // 3.555...

    // Basis vectors for the camera frame of reference
    w = unit_vector(look_from - look_at); // Vector on opposite direction to the gaze direction
    u = unit_vector(cross(view_up, w));
    v = cross(w, u);

    horizontal = focus_distance * viewport_width * u;
    vertical = focus_distance * viewport_height * v;
    lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_distance * w;
}

Ray Camera::get_ray(double s, double t) const
{
    Vector3 random = lens_radius * random_in_unit_disk();
    Vector3 offset = u * random.x() + v * random.y();

    return Ray{origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset, random_double(open_time, close_time)};
}

#endif // CAMERA_HPP