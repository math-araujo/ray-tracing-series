#include "camera.hpp"
#include "color.hpp"
#include "hittable_list.hpp"
#include "material.hpp"
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

    auto material_ground = std::make_shared<Lambertian>(Color{0.8, 0.8, 0.0});
    auto material_center = std::make_shared<Lambertian>(Color{0.1, 0.2, 0.5});
    auto material_left = std::make_shared<Dielectric>(1.5);
    auto material_right = std::make_shared<Metal>(Color{0.8, 0.6, 0.2}, 0.0);

    world.add(std::make_shared<Sphere>(Point3{0.0, -100.5, -1.0}, 100.0, material_ground));
    world.add(std::make_shared<Sphere>(Point3{0.0, 0.0, -1.0}, 0.5, material_center));
    world.add(std::make_shared<Sphere>(Point3{-1.0, 0.0, -1.0}, 0.5, material_left));
    world.add(std::make_shared<Sphere>(Point3{-1.0, 0.0, -1.0}, -0.45, material_left)); // Negative radius for hollow glass trick
    world.add(std::make_shared<Sphere>(Point3{1.0, 0.0, -1.0}, 0.5, material_right));

    /*
    Wide-angle view world settings
    const auto radius = std::cos(pi / 4);
    auto material_left = std::make_shared<Lambertian>(Color{0, 0, 1});
    auto material_right = std::make_shared<Lambertian>(Color{1, 0, 0});
    world.add(std::make_shared<Sphere>(Point3{-radius, 0, -1}, radius, material_left));
    world.add(std::make_shared<Sphere>(Point3{radius, 0, -1}, radius, material_right));
    */

    // Camera; set distance_to_focus to 1.0 and aperture to 0.0 to remove depth of field/defocus blur
    Point3 look_from{3, 3, 2};
    Point3 look_at{0, 0, -1};
    Vector3 view_up{0, 1, 0};
    auto vertical_fov = 20.0;
    auto distance_to_focus = (look_from - look_at).length();
    auto aperture = 2.0;
    Camera camera{look_from, look_at, view_up, vertical_fov, aspect_ratio, aperture, distance_to_focus};

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
        Ray scattered_ray;
        Color attenuation;
        if (record.material->scatter(ray, record, attenuation, scattered_ray))
        {
            return attenuation * ray_color(scattered_ray, world, depth - 1);
        }
        
        return Color{0, 0, 0};
    }

    Vector3 unit_direction = unit_vector(ray.direction());
    
    // unit.direction.y() ranges from -1.0 to 1.0, so lerp_parameter ranges from 0.0 to 1.0
    auto lerp_parameter = 0.5 * (unit_direction.y() + 1.0);

    Color white{1.0, 1.0, 1.0};
    Color light_blue{0.5, 0.7, 1.0};
    
    return (1 - lerp_parameter) * white + lerp_parameter * light_blue;
}