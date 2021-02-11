#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "perlin.hpp"
#include "vector3.hpp"
#include <cmath>

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

#endif // TEXTURE_HPP