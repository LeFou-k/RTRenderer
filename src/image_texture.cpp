#include "../include/image_texture.h"
#include "../include/rtw_stb_image.h"

image_texture::image_texture(const char* filename){
    auto components_per_pixel = bytes_per_pixel;

    data = stbi_load(filename, &width, &height, &components_per_pixel, components_per_pixel);

    if(!data){
        std::cerr << "ERROR: Could not load image file: '" << filename << "'\n";
        width = height = 0;
    }

    bytes_per_scanline = bytes_per_pixel * width;
}

color image_texture::value(double u, double v, const point3& p) const{
    //return cyan color as a debuging aid:
    if(!data) return color(0, 1, 1);

    u = clamp(u, 0.0, 1.0);
    v = 1.0 - clamp(v, 0.0, 1.0);

    auto i = static_cast<int>(width * u);
    auto j = static_cast<int>(height * v);

    if(i >= width) i = width - 1;
    if(j >= height) j = height - 1;

    const auto color_scale = 1.0 / 255;

    auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;

    return color(pixel[0], pixel[1], pixel[2]) * color_scale;
}