#include "hittable.hpp"
#include "material.hpp"
#include "sphere.hpp"
#include "vector3.hpp"
#include <memory>

enum class Scenes
{
    HollowGlass,
    Random,
    TwoCheckeredSpheres,
    PerlinTexture,
    PerlinTextureRandomSpheres,
    EarthSphere,
    SimpleLight,
    SimpleLightSphere,
    EmptyCornellBox,
};

// Simple scene developed along the "In One Weekend" book using lambertian, metal and dielectrics materials
HittableList hollow_glass_scene();

// Final render scene of "In One Weekend" book
HittableList random_scene(bool use_motion_blur = false);

// Two spheres with checkered textures
HittableList two_checkered_spheres();

// Two spheres with perlin textures
HittableList two_perlin_spheres();

// Similar to random_scene, but replaces solid textures with noise textures using Perlin noise
HittableList perlin_random_scene();

// Earth mapped to a sphere
HittableList earth_sphere();

// Rectangle as a light and a sphere with NoiseTexture
HittableList simple_light();

// Rectangle and sphere as light source and a sphere with NoiseTexture
HittableList simple_light_with_sphere();

// Empty Cornell Box
HittableList empty_cornell_box();

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
    const double diffuse_threshold = 0.8;
    const double metal_threshold = 0.95;

    HittableList world;

    /*auto ground_material = std::make_shared<Lambertian>(Color{0.5, 0.5, 0.5});
    world.add(std::make_shared<Sphere>(Point3{0, -1000, 0}, 1000, ground_material));*/
    auto checker_texture = std::make_shared<CheckerTexture>(Color{0.2, 0.3, 0.1}, Color{0.9, 0.9, 0.9});
    world.add(std::make_shared<Sphere>(Point3{0, -1000, 0}, 1000, std::make_shared<Lambertian>(checker_texture)));

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

HittableList two_checkered_spheres()
{
    HittableList objects;

    auto checker_texture = std::make_shared<CheckerTexture>(Color{0.2, 0.3, 0.1}, Color{0.9, 0.9, 0.9});
    
    objects.add(std::make_shared<Sphere>(Point3{0, -10, 0}, 10, std::make_shared<Lambertian>(checker_texture)));
    objects.add(std::make_shared<Sphere>(Point3{0, 10, 0}, 10, std::make_shared<Lambertian>(checker_texture)));

    return objects;
}

HittableList two_perlin_spheres()
{
    HittableList objects;

    auto perlin_texture = std::make_shared<NoiseTexture>(4);
    objects.add(std::make_shared<Sphere>(Point3{0, -1000, 0}, 1000, std::make_shared<Lambertian>(perlin_texture)));
    objects.add(std::make_shared<Sphere>(Point3{0, 2, 0}, 2, std::make_shared<Lambertian>(perlin_texture)));

    return objects;
}

HittableList perlin_random_scene()
{
    const double diffuse_threshold = 0.8;
    const double metal_threshold = 0.95;

    HittableList world;
    
    auto perlin_texture = std::make_shared<NoiseTexture>(4);
    world.add(std::make_shared<Sphere>(Point3{0, -1000, 0}, 1000, std::make_shared<Lambertian>(perlin_texture)));

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
                    sphere_material = std::make_shared<Lambertian>(perlin_texture);
                }
                else if (choose_material < metal_threshold) // Choose metal material
                {
                    auto albedo = Color::random(0.5, 1);
                    auto fuzziness = random_double(0, 0.5);
                    sphere_material = std::make_shared<Metal>(albedo, fuzziness);
                }
                else // Choose dielectric material
                {
                    sphere_material = std::make_shared<Dielectric>(1.5);
                }

                world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
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

HittableList earth_sphere()
{
    auto earth_texture = std::make_shared<ImageTexture>("earthmap.jpg");
    auto earth_surface = std::make_shared<Lambertian>(earth_texture);
    auto globe = std::make_shared<Sphere>(Point3{0, 0, 0}, 2, earth_surface);

    return HittableList{globe};
}

HittableList simple_light()
{
    HittableList objects;

    auto perlin_texture = std::make_shared<NoiseTexture>(4);
    objects.add(std::make_shared<Sphere>(Point3{0, -1000, 0}, 1000, std::make_shared<Lambertian>(perlin_texture)));
    objects.add(std::make_shared<Sphere>(Point3{0, 2, 0}, 2, std::make_shared<Lambertian>(perlin_texture)));

    auto diffuse_light = std::make_shared<DiffuseLight>(Color{4, 4, 4});
    objects.add(std::make_shared<XYRect>(3, 5, 1, 3, -2, diffuse_light));

    return objects;
}

HittableList simple_light_with_sphere()
{
    HittableList objects;

    auto perlin_texture = std::make_shared<NoiseTexture>(4);
    objects.add(std::make_shared<Sphere>(Point3{0, -1000, 0}, 1000, std::make_shared<Lambertian>(perlin_texture)));
    objects.add(std::make_shared<Sphere>(Point3{0, 2, 0}, 2, std::make_shared<Lambertian>(perlin_texture)));

    auto diffuse_light = std::make_shared<DiffuseLight>(Color{4, 4, 4});
    objects.add(std::make_shared<XYRect>(3, 5, 1, 3, -2, diffuse_light));
    objects.add(std::make_shared<Sphere>(Point3{0, 7, 0}, 2, diffuse_light));

    return objects;
}

HittableList empty_cornell_box()
{
    HittableList objects;

    auto red = std::make_shared<Lambertian>(Color{0.65, 0.05, 0.05});
    auto white = std::make_shared<Lambertian>(Color{0.73, 0.73, 0.73});
    auto green = std::make_shared<Lambertian>(Color{0.12, 0.45, 0.15});
    auto light = std::make_shared<DiffuseLight>(Color{15, 15, 15});

    objects.add(std::make_shared<YZRect>(0, 555, 0, 555, 555, green));
    objects.add(std::make_shared<YZRect>(0, 555, 0, 555, 0, red));
    objects.add(std::make_shared<XZRect>(213, 343, 227, 332, 554, light));
    objects.add(std::make_shared<XZRect>(0, 555, 0, 555, 0, white));
    objects.add(std::make_shared<XZRect>(0, 555, 0, 555, 555, white));
    objects.add(std::make_shared<XYRect>(0, 555, 0, 555, 555, white));

    return objects;
}