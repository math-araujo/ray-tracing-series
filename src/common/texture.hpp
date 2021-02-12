#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "perlin.hpp"
#include "rt_stb_image.hpp"
#include "vector3.hpp"
#include <cmath>
#include <vector>

class Texture
{
public:
    virtual Color value(double u, double v, const Point3& point) const = 0;
};

class SolidColor: public Texture
{
public:
    SolidColor() {}
    SolidColor(Color color): color_value{color} {}
    SolidColor(double red, double green, double blue): SolidColor(Color{red, green, blue}) {}

    virtual Color value(double u, double v, const Vector3& vector) const override;
private:
    Color color_value;
};

Color SolidColor::value(double u, double v, const Vector3& vector) const 
{
    return color_value;
}

class CheckerTexture: public Texture
{
public:
    std::shared_ptr<Texture> odd;
    std::shared_ptr<Texture> even;

    CheckerTexture() {}
    CheckerTexture(std::shared_ptr<Texture> even_texture, std::shared_ptr<Texture> odd_texture): 
                   even{even_texture}, odd{odd_texture} {}
    
    CheckerTexture(Color color1, Color color2): 
                   even{std::make_shared<SolidColor>(color1)}, odd{std::make_shared<SolidColor>(color2)} {}

    virtual Color value(double u, double v, const Point3& point) const override;
};

Color CheckerTexture::value(double u, double v, const Point3& point) const 
{
    auto sines = std::sin(10 * point.x()) * std::sin(10 * point.y()) * std::sin(10 * point.z());
    if (sines < 0)
    {
        return odd->value(u, v, point);
    }
    
    return even->value(u, v, point);
}

class NoiseTexture: public Texture
{
public:
    Perlin noise;
    double frequency_scale;

    NoiseTexture() {}
    explicit NoiseTexture(double scale): frequency_scale{scale} {}

    virtual Color value(double u, double v, const Point3& point) const override;
};

Color NoiseTexture::value(double u, double v, const Point3& point) const 
{
    return Color{1, 1, 1} * 0.5 * (1 + std::sin(frequency_scale * point.z() + 10 * noise.turbulence(point)));
}

class ImageTexture: public Texture
{
public:
    constexpr static int bytes_per_pixel{3};
    
    ImageTexture() {}
    ImageTexture(const std::string& filename);

    virtual Color value(double u, double v, const Vector3& point) const override;
private:
    std::vector<unsigned char> image;
    int width{0};
    int height{0};
    int bytes_per_scanline{0};
};

ImageTexture::ImageTexture(const std::string& filename)
{
    auto components_per_pixel = bytes_per_pixel;

    auto data_ptr = stbi_load(filename.c_str(), &width, &height, &components_per_pixel, components_per_pixel);

    if (!data_ptr)
    {
        std::cerr << "ERROR: could not load texture image file " << filename << ".\n";
        width = 0;
        height = 0;
    }
    
    image = std::vector<unsigned char>(data_ptr, data_ptr + width * height * bytes_per_pixel);
    delete data_ptr;
    bytes_per_scanline = bytes_per_pixel * width;
}

Color ImageTexture::value(double u, double v, const Point3& point) const 
{
    if (image.empty()) // No texture
    {
        return Color{0, 1, 1}; // Return solid cyan for debugging
    }

    // Clamp textures to [0; 1] x [1; 0] range
    u = clamp(u, 0.0, 1.0);
    v = 1.0 - clamp(v, 0.0, 1.0);

    auto i = static_cast<int>(u * width);
    auto j = static_cast<int>(v * height);

    if (i >= width)
    {
        i = width - 1;
    }

    if (j >= height)
    {
        j = height - 1;
    }

    const auto color_space = 1.0 / 255.0;
    const int pixel_index = j * bytes_per_scanline + i * bytes_per_pixel;
    const auto red = image[pixel_index + 0];
    const auto green = image[pixel_index + 1];
    const auto blue = image[pixel_index + 2];

    return Color{color_space * red, color_space * green, color_space * blue};
}

#endif // TEXTURE_HPP