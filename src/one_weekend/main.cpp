#include "color.hpp"
#include "hittable_list.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "util.hpp"
#include "vector3.hpp"

#include <iostream>

Color ray_color(const Ray& ray, const Hittable& world);

int main()
{
    // Image settings
    constexpr double aspect_ratio = 16.0 / 9.0;
    constexpr int image_width = 400;
    constexpr int image_height = static_cast<int>(image_width / aspect_ratio); // 225

    // World
    HittableList world;
    world.add(std::make_shared<Sphere>(Point3{0, 0, -1}, 0.5)); // Main sphere
    world.add(std::make_shared<Sphere>(Point3{0, -100.5, -1}, 100)); // Background sphere

    // Camera
    double viewport_height = 2.0;
    double viewport_width = aspect_ratio * viewport_height; // 3.555...
    double focal_length = 1.0; // Distance from the eyepoint to the projection plane

    Point3 origin{0, 0, 0};
    Vector3 horizontal{viewport_width, 0, 0};
    Vector3 vertical{0, viewport_height, 0};
    auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vector3{0, 0, focal_length};

    // Render
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
    
    // Starting at upper left corner, left to right and up to bottom
    for (int row = image_height - 1; row >= 0; --row)
    {
        std::cerr << "\rScanlines remaining: " << row << ' ' << std::flush;

        for (int column = 0; column < image_width; ++column)
        {
            auto u = double(column) / (image_width - 1);
            auto v = double(row) / (image_height - 1);
            
            Ray ray{origin, lower_left_corner + u * horizontal + v * vertical - origin};
            Color pixel_color = ray_color(ray, world);
            
            write_color(std::cout, pixel_color);
        }
    }
    
    std::cerr << "\nDone.\n";
}

Color ray_color(const Ray& ray, const Hittable& world)
{
    HitRecord record;

    if (world.hit(ray, 0, infinity, record))
    {
        // record.normal coordinates are between -1 and 1, so this returns RGB between 0 and 1
        return 0.5 * (record.normal + Color{1, 1, 1});
    }

    Vector3 unit_direction = unit_vector(ray.direction());
    
    // unit.direction.y() ranges from -1.0 to 1.0, so lerp_parameter ranges from 0.0 to 1.0
    auto lerp_parameter = 0.5 * (unit_direction.y() + 1.0);

    Color white{1.0, 1.0, 1.0};
    Color light_blue{0.5, 0.7, 1.0};
    
    return (1 - lerp_parameter) * white + lerp_parameter * light_blue;

    /*const Point3 sphere_center{0, 0, -1};
    const double radius = 0.5;
    
    // Root of the ray-sphere intersection equation, if any
    auto intersection_parameter = hit_sphere(sphere_center, radius, ray);
    if (intersection_parameter > 0.0)
    {
        Vector3 normal = unit_vector(ray.at(intersection_parameter) - sphere_center);
        return 0.5 * Color{normal.x() + 1, normal.y() + 1, normal.z() + 1};
    }
    
    Vector3 unit_direction = unit_vector(ray.direction());
    
    // unit.direction.y() ranges from -1.0 to 1.0, so lerp_parameter ranges from 0.0 to 1.0
    auto lerp_parameter = 0.5 * (unit_direction.y() + 1.0);

    Color white{1.0, 1.0, 1.0};
    Color light_blue{0.5, 0.7, 1.0};
    
    return (1 - lerp_parameter) * white + lerp_parameter * light_blue;*/
}