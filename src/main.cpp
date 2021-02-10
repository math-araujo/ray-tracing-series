#include "camera.hpp"
#include "color.hpp"
#include "hittable_list.hpp"
#include "material.hpp"
#include "moving_sphere.hpp"
#include "ray.hpp"
#include "scenes.hpp"
#include "sphere.hpp"
#include "util.hpp"
#include "vector3.hpp"

#include <iostream>

// Simple scene developed along the "In One Weekend" book using lambertian, metal and dielectrics materials
HittableList hollow_glass_scene();

// Final render scene of "In One Weekend" book
HittableList random_scene(bool use_motion_blur = false);

// Recursive ray tracing function to compute color for a pixel
Color ray_color(const Ray& ray, const Hittable& world, int depth);

int main()
{
    // Image settings
    constexpr double aspect_ratio = 16.0 / 9.0;
    constexpr int image_width = 400;
    constexpr int image_height = static_cast<int>(image_width / aspect_ratio); // 800
    constexpr int samples_per_pixel = 100;
    constexpr int max_depth = 50;

    /*
    Wide-angle view world settings
    const auto radius = std::cos(pi / 4);
    auto material_left = std::make_shared<Lambertian>(Color{0, 0, 1});
    auto material_right = std::make_shared<Lambertian>(Color{1, 0, 0});
    world.add(std::make_shared<Sphere>(Point3{-radius, 0, -1}, radius, material_left));
    world.add(std::make_shared<Sphere>(Point3{radius, 0, -1}, radius, material_right));
    */

    // Camera settings; set distance_to_focus to 1.0 and aperture to 0.0 to remove depth of field/defocus blur
    Point3 look_from;
    Point3 look_at;
    Vector3 view_up{0, 1, 0};
    double vertical_fov{20.0};
    double distance_to_focus{0.0};
    double aperture{0.1};

    auto choosen_scene{Scenes::Random};
    bool use_motion_blur{true};
    HittableList world;

    switch (choosen_scene)
    {
    case Scenes::HollowGlass:
        look_from = Point3{3, 3, 2};
        look_at = Point3{0, 0, -1};
        aperture = 0.1;
        distance_to_focus = (look_from - look_at).length();
        world = hollow_glass_scene();
        break;
    case Scenes::Random:
        look_from = Point3{13, 2, 3};
        look_at = {0, 0, 0};
        aperture = 0.1;
        distance_to_focus = 10.0;
        world = random_scene(use_motion_blur);
        break;
    default:
        std::cerr << "Empty scene: unable to render\n";
        return 1;
    }

    double open_shutter_time{0.0};
    double close_shutter_time{1.0};
    Camera camera{look_from, look_at, view_up, vertical_fov, aspect_ratio, aperture, distance_to_focus, open_shutter_time, close_shutter_time};

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

HittableList hollow_glass_scene()
{
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

    return world;
}

HittableList random_scene(bool use_motion_blur)
{   
    constexpr double diffuse_threshold = 0.8;
    constexpr double metal_threshold = 0.95;

    HittableList world;

    auto ground_material = std::make_shared<Lambertian>(Color{0.5, 0.5, 0.5});
    world.add(std::make_shared<Sphere>(Point3{0, -1000, 0}, 1000, ground_material));

    for (int a = -11; a < 11; ++a)
    {
        for (int b = -11; b < 11; ++b)
        {
            Point3 center{a + 0.9 * random_double(), 0.2, b + 0.9 * random_double()};

            if ((center - Point3{4, 0.2, 0}).length() > 0.9)
            {
                std::shared_ptr<Material> sphere_material;

                auto choose_material = random_double();

                if (choose_material < diffuse_threshold) // Choose diffuse material
                {
                    auto albedo = Color::random() * Color::random();
                    sphere_material = std::make_shared<Lambertian>(albedo);

                    if (use_motion_blur)
                    {
                        auto end_center = center + Vector3{0, random_double(0, 0.5), 0};
                        world.add(std::make_shared<MovingSphere>(center, end_center, 0.0, 1.0, 0.2, sphere_material));
                    }
                    else
                    {
                        world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                    }
                }
                else if (choose_material < metal_threshold) // Choose metal material
                {
                    auto albedo = Color::random(0.5, 1);
                    auto fuzziness = random_double(0, 0.5);
                    sphere_material = std::make_shared<Metal>(albedo, fuzziness);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else // Choose dielectric material
                {
                    sphere_material = std::make_shared<Dielectric>(1.5);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto dielectric = std::make_shared<Dielectric>(1.5);
    world.add(std::make_shared<Sphere>(Point3{0, 1, 0}, 1.0, dielectric));

    auto lambertian = std::make_shared<Lambertian>(Color{0.4, 0.2, 0.1});
    world.add(std::make_shared<Sphere>(Point3{-4, 1, 0}, 1.0, lambertian));

    auto metal = std::make_shared<Metal>(Color{0.7, 0.6, 0.5}, 0.0);
    world.add(std::make_shared<Sphere>(Point3{4, 1, 0}, 1.0, metal));

    return world;
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