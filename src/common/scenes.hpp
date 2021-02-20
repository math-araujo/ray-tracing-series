#include "box.hpp"
#include "bvh.hpp"
#include "constant_medium.hpp"
#include "hittable.hpp"
#include "material.hpp"
#include "sphere.hpp"
#include "transform.hpp"
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
    TwoBlocksCornellBox,
    ClassicCornellBox,    
    SmokeCornellBox,
    NextWeekFinal,
    // Custom scenes
    WikipediaPathTracing,    
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

// Initial Cornell Box with two blocks (unrotated)
HittableList two_blocks_cornell_box();

// Classic Cornell Box with two rotated blocks
HittableList classic_cornell_box();

// Cornell Box with blocks replaced by smoke
HittableList smoke_cornell_box();

// Final scene of Book 2 - Next Week: BVH, mist, textures, emissive materials, motion blur
HittableList next_week_final_scene();

/*
Attempt to partially recreate the image on Wikipedia "Path Tracing" entry
Source:  https://upload.wikimedia.org/wikipedia/commons/archive/e/e0/20160706024146%21Path_tracing_001.png

Arguments:
    ambient_light: if ambient_light is true, no diffuse light source is added to the scene
    and it's assumed that the light comes from the background.
*/
HittableList wikipedia_path_tracing_scene(bool ambient_light = true);

// Scenes definitions

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

HittableList two_blocks_cornell_box()
{
    HittableList objects;

    auto red = std::make_shared<Lambertian>(Color{0.65, 0.05, 0.05});
    auto white = std::make_shared<Lambertian>(Color{0.73, 0.73, 0.73});
    auto green = std::make_shared<Lambertian>(Color{0.12, 0.45, 0.15});
    auto light = std::make_shared<DiffuseLight>(Color{15, 15, 15});

    // Walls and light source
    objects.add(std::make_shared<YZRect>(0, 555, 0, 555, 555, green));
    objects.add(std::make_shared<YZRect>(0, 555, 0, 555, 0, red));
    objects.add(std::make_shared<XZRect>(213, 343, 227, 332, 554, light));
    objects.add(std::make_shared<XZRect>(0, 555, 0, 555, 555, white));
    objects.add(std::make_shared<XZRect>(0, 555, 0, 555, 0, white));
    objects.add(std::make_shared<XYRect>(0, 555, 0, 555, 555, white));

    // Instance blocks
    objects.add(std::make_shared<Box>(Point3{130, 0, 65}, Point3{295, 165, 230}, white));
    objects.add(std::make_shared<Box>(Point3{265, 0, 295}, Point3{430, 330, 460}, white));

    return objects;
}

HittableList classic_cornell_box()
{
    HittableList objects;

    auto red = std::make_shared<Lambertian>(Color{0.65, 0.05, 0.05});
    auto white = std::make_shared<Lambertian>(Color{0.73, 0.73, 0.73});
    auto green = std::make_shared<Lambertian>(Color{0.12, 0.45, 0.15});
    auto light = std::make_shared<DiffuseLight>(Color{15, 15, 15});

    // Walls and light source
    objects.add(std::make_shared<YZRect>(0, 555, 0, 555, 555, green));
    objects.add(std::make_shared<YZRect>(0, 555, 0, 555, 0, red));
    objects.add(std::make_shared<XZRect>(213, 343, 227, 332, 554, light));
    objects.add(std::make_shared<XZRect>(0, 555, 0, 555, 555, white));
    objects.add(std::make_shared<XZRect>(0, 555, 0, 555, 0, white));
    objects.add(std::make_shared<XYRect>(0, 555, 0, 555, 555, white));

    // Instance blocks
    std::shared_ptr<Hittable> box1 = std::make_shared<Box>(Point3{0, 0, 0}, Point3{165, 330, 165}, white);
    box1 = std::make_shared<RotateY>(box1, 15);
    box1 = std::make_shared<Translate>(box1, Vector3{265, 0, 295});
    objects.add(box1);

    std::shared_ptr<Hittable> box2 = std::make_shared<Box>(Point3{0, 0, 0}, Point3{165, 165, 165}, white);
    box2 = std::make_shared<RotateY>(box2, -18);
    box2 = std::make_shared<Translate>(box2, Vector3{130, 0, 65});
    objects.add(box2);

    return objects;
}

HittableList smoke_cornell_box()
{
    HittableList objects;

    auto red = std::make_shared<Lambertian>(Color{0.65, 0.05, 0.05});
    auto white = std::make_shared<Lambertian>(Color{0.73, 0.73, 0.73});
    auto green = std::make_shared<Lambertian>(Color{0.12, 0.45, 0.15});
    auto light = std::make_shared<DiffuseLight>(Color{7, 7, 7});

    // Walls and light source
    objects.add(std::make_shared<YZRect>(0, 555, 0, 555, 555, green));
    objects.add(std::make_shared<YZRect>(0, 555, 0, 555, 0, red));
    objects.add(std::make_shared<XZRect>(113, 443, 127, 432, 554, light));
    objects.add(std::make_shared<XZRect>(0, 555, 0, 555, 555, white));
    objects.add(std::make_shared<XZRect>(0, 555, 0, 555, 0, white));
    objects.add(std::make_shared<XYRect>(0, 555, 0, 555, 555, white));

    // Smoke blocks
    std::shared_ptr<Hittable> box1 = std::make_shared<Box>(Point3{0, 0, 0}, Point3{165, 330, 165}, white);
    box1 = std::make_shared<RotateY>(box1, 15);
    box1 = std::make_shared<Translate>(box1, Vector3{265, 0, 295});  

    std::shared_ptr<Hittable> box2 = std::make_shared<Box>(Point3{0, 0, 0}, Point3{165, 165, 165}, white);
    box2 = std::make_shared<RotateY>(box2, -18);
    box2 = std::make_shared<Translate>(box2, Vector3{130, 0, 65});
    
    objects.add(std::make_shared<ConstantMedium>(box1, 0.01, Color{0, 0, 0}));
    objects.add(std::make_shared<ConstantMedium>(box2, 0.01, Color{1, 1, 1}));

    return objects;
}

HittableList next_week_final_scene()
{
    HittableList ground_boxes;
    auto ground_material = std::make_shared<Lambertian>(Color{0.48, 0.83, 0.53});
    
    const int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; ++i)
    {
        for (int j = 0; j < boxes_per_side; ++j)
        {
            auto w = 100.0;
            auto x0 = -1000.0 + i * w;
            auto z0 = -1000.0 + j * w;
            auto y0 = 0.0;

            auto x1 = x0 + w;
            auto y1 = random_double(1, 101);
            auto z1 = z0 + w;

            ground_boxes.add(std::make_shared<Box>(Point3{x0, y0, z0}, Point3{x1, y1, z1}, ground_material));
        }
    }

    HittableList objects;

    objects.add(std::make_shared<BVHNode>(ground_boxes, 0, 1));

    auto light = std::make_shared<DiffuseLight>(Color{7, 7, 7});
    objects.add(std::make_shared<XZRect>(123, 423, 147, 412, 554, light));

    // Sphere with motion blur
    Point3 initial_center{400, 400, 200};
    Point3 final_center = initial_center + Vector3{30, 0, 0};
    auto moving_sphere_material = std::make_shared<Lambertian>(Color{0.7, 0.3, 0.1});
    objects.add(std::make_shared<MovingSphere>(initial_center, final_center, 0, 1, 50, moving_sphere_material));
    
    // Glass sphere, bottom center
    objects.add(std::make_shared<Sphere>(Point3{260, 150, 45}, 50, std::make_shared<Dielectric>(1.5)));
    // Metallic sphere, bottom right
    objects.add(std::make_shared<Sphere>(Point3{0, 150, 145}, 50, std::make_shared<Metal>(Color{0.8, 0.8, 0.9}, 1.0)));

    // Blue sphere: formally this is subsurface scattering or reflection
    // This is implemented as a volume (e.g. ConstantMedium) inside a dielectric
    // Subsurface scattering Step 1: standard glass sphere
    auto sphere_boundary = std::make_shared<Sphere>(Point3{360, 150, 145}, 70, std::make_shared<Dielectric>(1.5));
    objects.add(sphere_boundary);
    // Subsurface scattering Step 2: add a blue volume inside the boundary (i.e. the sphere)
    objects.add(std::make_shared<ConstantMedium>(sphere_boundary, 0.2, Color{0.2, 0.4, 0.9}));

    // Ambient mist: a sphere around the entire ambient is used as a boundary and the volume is spread inside
    auto ambient_boundary = std::make_shared<Sphere>(Point3{0, 0, 0}, 5000, std::make_shared<Dielectric>(1.5));
    objects.add(std::make_shared<ConstantMedium>(ambient_boundary, 0.0001, Color{1, 1, 1}));

    // Spheres with textures
    auto earth_material = std::make_shared<Lambertian>(std::make_shared<ImageTexture>("earthmap.jpg"));
    objects.add(std::make_shared<Sphere>(Point3{400, 200, 400}, 100, earth_material));

    auto perlin_texture = std::make_shared<NoiseTexture>(0.1);
    objects.add(std::make_shared<Sphere>(Point3{220, 280, 300}, 80, std::make_shared<Lambertian>(perlin_texture)));

    // Cotton box
    HittableList cotton_box;
    auto white = std::make_shared<Lambertian>(Color{0.73, 0.73, 0.73});
    const int number_of_spheres = 1000;
    for (int i = 0; i < number_of_spheres; ++i)
    {
        cotton_box.add(std::make_shared<Sphere>(Point3::random(0, 165), 10, white));
    }

    objects.add(std::make_shared<Translate>(std::make_shared<RotateY>(std::make_shared<BVHNode>(cotton_box, 0.0, 1.0), 15), Vector3{-100, 270, 395}));

    return objects;
}

HittableList wikipedia_path_tracing_scene(bool ambient_light)
{
    HittableList world;

    const Color white{0.8, 0.8, 0.8};
    const Color red{1.0, 0.0, 0.0};
    const Color green{0.0, 1.0, 0.0};
    const Color blue{0.0, 0.0, 1.0};
    const Color salmon{1.0, 0.501960784, 0.4};

    auto white_material = std::make_shared<Lambertian>(white);
    auto salmon_material = std::make_shared<Lambertian>(salmon);
    world.add(std::make_shared<Sphere>(Point3{0, -1000, 0}, 1000, salmon_material));

    if (!ambient_light)
    {
        world.add(std::make_shared<Translate>(std::make_shared<YZRect>(-5, 5, -5, 5, -20, std::make_shared<DiffuseLight>(Color{10, 10, 10})), Vector3{0, 20, 0}));
    }

    auto red_material = std::make_shared<Lambertian>(red);
    auto green_material = std::make_shared<Lambertian>(green);
    auto blue_material = std::make_shared<Lambertian>(blue);

    auto no_fuzz_metal = std::make_shared<Metal>(white, 0.0);
    world.add(std::make_shared<Sphere>(Point3{-8.0, 7, 0}, 1.0, no_fuzz_metal)); // left floating sphere
    
    world.add(std::make_shared<Sphere>(Point3{-6.5, 1, 0}, 1.0, red_material));
    world.add(std::make_shared<Sphere>(Point3{-6.5, 2, -3}, 2.0, white_material));

    world.add(std::make_shared<Box>(Point3{-5.0, 0.0, -1.0}, Point3{-1.5, 0.25, 1.0}, white_material));
    world.add(std::make_shared<Sphere>(Point3{-3.25, 1.25, -0.0}, 1.0, white_material));

    auto central_metal = std::make_shared<Metal>(Color{1, 1, 1}, 0.4);
    world.add(std::make_shared<Sphere>(Point3{0, 1, 0}, 1.0, blue_material));
    world.add(std::make_shared<Sphere>(Point3{0, 2, -3}, 2.0, central_metal));
    world.add(std::make_shared<Sphere>(Point3{0, 7, 0}, 1.0, no_fuzz_metal)); // center floating sphere

    world.add(std::make_shared<Box>(Point3{1.5, 0.0, -1.0}, Point3{5.0, 0.25, 1.0}, white_material));
    world.add(std::make_shared<Sphere>(Point3{3.25, 1.25, 0.0}, 1.0, no_fuzz_metal));

    world.add(std::make_shared<Sphere>(Point3{6.5, 1, 0}, 1.0, green_material));
    world.add(std::make_shared<Sphere>(Point3{6.5, 2, -3}, 2.0, no_fuzz_metal));
    world.add(std::make_shared<Sphere>(Point3{8.0, 7, 0}, 1.0, no_fuzz_metal)); // right floating sphere

    return world;
}