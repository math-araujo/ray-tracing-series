#include "aarect.hpp"
#include "camera.hpp"
#include "color.hpp"
#include "hittable_list.hpp"
#include "material.hpp"
#include "moving_sphere.hpp"
#include "ray.hpp"
#include "scenes.hpp"
#include "sphere.hpp"
#include "texture.hpp"
#include "util.hpp"
#include "vector3.hpp"
#include <iostream>

// Recursive ray tracing function to compute color for a pixel
Color ray_color(const Ray& ray, const Hittable& world, int depth);

// Overloading of ray_color to accept a single background color instead of a gradient
Color ray_color(const Ray& ray, const Color& background, const Hittable& world, int depth);

int main()
{
    // Image settings
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 1200;
    int image_height = static_cast<int>(image_width / aspect_ratio); // 675
    int samples_per_pixel = 100;
    int max_depth = 50;

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
    double distance_to_focus{1.0};
    double aperture{0.0};

    auto choosen_scene{Scenes::PointCloud};
    bool use_motion_blur{true};
    HittableList world;
    Color background{0, 0, 0};

    switch (choosen_scene)
    {
    case Scenes::HollowGlass:
        look_from = Point3{3, 3, 2};
        look_at = Point3{0, 0, -1};
        aperture = 0.1;
        distance_to_focus = (look_from - look_at).length();
        world = hollow_glass_scene();
        background = Color{0.70, 0.80, 1.00};
        break;
    case Scenes::Random:
        look_from = Point3{13, 2, 3};
        look_at = Point3{0, 0, 0};
        aperture = 0.1;
        distance_to_focus = 10.0;
        world = random_scene(use_motion_blur);
        background = Color{0.70, 0.80, 1.00};
        break;
    case Scenes::TwoCheckeredSpheres:
        look_from = Point3{13, 2, 3};
        look_at = Point3{0, 0, 0};
        distance_to_focus = 10.0;
        world = two_checkered_spheres();
        background = Color{0.70, 0.80, 1.00};
        break;
    case Scenes::PerlinTexture:
        look_from = Point3{13, 2, 3};
        look_at = Point3{0, 0, 0};
        distance_to_focus = 10.0;
        world = two_perlin_spheres();
        background = Color{0.70, 0.80, 1.00};
        break;
    case Scenes::PerlinTextureRandomSpheres:
        look_from = Point3{13, 2, 3};
        look_at = Point3{0, 0, 0};
        aperture = 0.1;
        distance_to_focus = 10.0;
        world = perlin_random_scene();
        background = Color{0.70, 0.80, 1.00};
        break;
    case Scenes::EarthSphere:
        look_from = Point3{13, 2, 3};
        look_at = Point3{0, 0, 0};
        world = earth_sphere();
        background = Color{0.70, 0.80, 1.00};
        break;
    case Scenes::SimpleLight:
        look_from = Point3{26, 3, 6};
        look_at = Point3{0, 2, 0};
        world = simple_light();
        break;
    case Scenes::SimpleLightSphere:
        look_from = Point3{26, 3, 6};
        look_at = Point3{0, 2, 0};
        world = simple_light_with_sphere();
        break;
    case Scenes::EmptyCornellBox:
        aspect_ratio = 1.0;
        image_width = 600;
        image_height = static_cast<int>(image_width / aspect_ratio);
        look_from = Point3{278, 278, -800};
        look_at = Point3{278, 278, 0};
        vertical_fov = 40.0;
        world = empty_cornell_box();
        break;
    case Scenes::TwoBlocksCornellBox:
        aspect_ratio = 1.0;
        image_width = 600;
        image_height = static_cast<int>(image_width / aspect_ratio);
        samples_per_pixel = 200;
        look_from = Point3{278, 278, -800};
        look_at = Point3{278, 278, 0};
        vertical_fov = 40.0;
        world = two_blocks_cornell_box();
        break;
    case Scenes::ClassicCornellBox:
        aspect_ratio = 1.0;
        image_width = 600;
        image_height = static_cast<int>(image_width / aspect_ratio);
        samples_per_pixel = 200;
        look_from = Point3{278, 278, -800};
        look_at = Point3{278, 278, 0};
        vertical_fov = 40.0;
        world = classic_cornell_box();
        break;
    case Scenes::SmokeCornellBox:
        aspect_ratio = 1.0;
        image_width = 600;
        image_height = static_cast<int>(image_width / aspect_ratio);
        samples_per_pixel = 200;
        look_from = Point3{278, 278, -800};
        look_at = Point3{278, 278, 0};
        vertical_fov = 40.0;
        world = smoke_cornell_box();
        break;
    case Scenes::NextWeekFinal:
        aspect_ratio = 1.0;
        image_width = 800;
        image_height = static_cast<int>(image_width / aspect_ratio);
        samples_per_pixel = 1000;
        look_from = Point3{478, 278, -600};
        look_at = Point3{278, 278, 0};
        vertical_fov = 40.0;
        world = next_week_final_scene();
        break;
    case Scenes::WikipediaPathTracing:
        aspect_ratio = 3.0 / 2.0;
        image_height = static_cast<int>(image_width / aspect_ratio); // 800
        look_from = Point3{0, 13, 30};
        look_at = Point3{0, 3, 0};
        background = Color{1.0, 1.0, 1.0};
        //background = Color{0.0, 0.0, 0.0};
        if (background != Color{0, 0, 0})
        {
            samples_per_pixel = 200;
            world = wikipedia_path_tracing_scene();
        }
        else
        {
            samples_per_pixel = 500; // More rays are needed because of the small light source
            world = wikipedia_path_tracing_scene(false);
        }
        break;
    case Scenes::RecursiveGlass:
        look_from = Point3{0, 1, 6};
        look_at = Point3{0, 1, 0};
        world = recursive_glass();
        background = Color{1.0, 1.0, 1.0};
        break;
    case Scenes::PointCloud:
        aspect_ratio = 3.0 / 2.0;
        image_height = static_cast<int>(image_width / aspect_ratio); // 800
        look_from = Point3{0, 4, 10};
        look_at = Point3{-0.25, 1.5, 0};
        aperture = 0.0;
        distance_to_focus = 1.0;
        //background = Color{0.70, 0.80, 1.00};
        background = Color{0, 0, 0};
        world = point_cloud(background != Color{0, 0, 0}); 
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
                //pixel_color += ray_color(ray, world, max_depth); // gradient-sky background
                pixel_color += ray_color(ray, background, world, max_depth);
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

Color ray_color(const Ray& ray, const Color& background, const Hittable& world, int depth)
{
    if (depth <= 0)
    {
        return Color{0, 0, 0};
    }

    HitRecord record;
    if (!world.hit(ray, 0.001, infinity, record))
    {
        return background;
    }

    Ray scattered_ray;
    Color attenuation;
    Color emitted = record.material->emitted(record.u, record.v, record.point);

    if (!record.material->scatter(ray, record, attenuation, scattered_ray))
    {
        return emitted;
    }

    return emitted + attenuation * ray_color(scattered_ray, background, world, depth - 1);
}