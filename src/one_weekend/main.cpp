#include "camera.hpp"
#include "color.hpp"
#include "hittable_list.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "util.hpp"
#include "vector3.hpp"

#include <iostream>

Color ray_color(const Ray& ray, const Hittable& world, int depth);

int main()
{
    // Image settings
    constexpr double aspect_ratio = 16.0 / 9.0;
    constexpr int image_width = 400;
    constexpr int image_height = static_cast<int>(image_width / aspect_ratio); // 225
    constexpr int samples_per_pixel = 100;
    constexpr int max_depth = 50;

    // World
    HittableList world;
    world.add(std::make_shared<Sphere>(Point3{0, 0, -1}, 0.5)); // Main sphere
    world.add(std::make_shared<Sphere>(Point3{0, -100.5, -1}, 100)); // Background sphere

    // Camera
    Camera camera;

    // Render
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
    
    // Starting at upper left corner, left to right and up to bottom
    for (int row = image_height - 1; row >= 0; --row)
    {
        std::cerr << "\rScanlines remaining: " << row << ' ' << std::flush;

        for (int column = 0; column < image_width; ++column)
        {
            Color pixel_color{0.0, 0.0, 0.0};
            for (int sample = 0; sample < samples_per_pixel; ++sample)
            {
                auto u = (column + random_double()) / (image_width - 1);
                auto v = (row + random_double()) / (image_height - 1);

                Ray ray = camera.get_ray(u, v);
                pixel_color += ray_color(ray, world, max_depth);
            }

            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
    
    std::cerr << "\nDone.\n";
}

Color ray_color(const Ray& ray, const Hittable& world, int depth)
{
    if (depth == 0)
    {
        return Color{0.0, 0.0, 0.0};
    }

    HitRecord record;

    if (world.hit(ray, 0.001, infinity, record))
    {
        // Random point inside unit sphere tangent to the surface at the hit point
        Point3 target = record.point + record.normal + random_in_unit_sphere();
        
        /*
        Generate ray from the hit point to the random target point.
        Recursively computes the color of the pixel from the random ray reflection.
        */
        return 0.5 * ray_color(Ray{record.point, target - record.point}, world, depth - 1);
    }

    Vector3 unit_direction = unit_vector(ray.direction());
    
    // unit.direction.y() ranges from -1.0 to 1.0, so lerp_parameter ranges from 0.0 to 1.0
    auto lerp_parameter = 0.5 * (unit_direction.y() + 1.0);

    Color white{1.0, 1.0, 1.0};
    Color light_blue{0.5, 0.7, 1.0};
    
    return (1 - lerp_parameter) * white + lerp_parameter * light_blue;
}