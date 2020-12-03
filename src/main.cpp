#include "../include/rtweekend.h"
#include "../include/color.h"
#include "../include/sphere.h"
#include "../include/hittablelist.h"
#include "../include/camera.h"
#include "../include/lambertian.h"
#include "../include/metal.h"
#include "../include/dielectric.h"
#include "../include/moving_sphere.h"
#include "../include/bvh.h"
#include "../include/checker.h"
#include "../include/noise.h"
#include "../include/image_texture.h"
#include "../include/diffuse_light.h"
#include "../include/xy_rect.h"
#include "../include/xz_rect.h"
#include "../include/yz_rect.h"
#include "../include/box.h"
#include "../include/rotate.h"
#include "../include/translate.h"


#include <omp.h>
#include <iostream>
#include <cmath>
#include <fstream>
#include <chrono>
#include <iomanip>


//传递一个hittable_list，返回光线上最近的位置t
color rayColor(Ray ray, const color &background, const hittable &world, int depth) {
    if (depth <= 0) return color(0, 0, 0);

    hit_record rec;
    if (!world.hit(ray, 0.001, infinity, rec))
        return background;

    Ray scattered;
    color attenuation;
    color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(ray, rec, attenuation, scattered))
        return emitted;

    return emitted + attenuation * rayColor(scattered, background, world, depth - 1);
}


//generate random sphere scene:
hittable_list random_scene() {
    hittable_list world;
    auto ground_material = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(ground_material)));

    for (int a = -11; a < 11; ++a) {
        for (int b = -11; b < 11; ++b) {
            auto choose_material = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_material < 0.8) {
                    //diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    auto center2 = center + vec3(0, random_double(0, 0.5), 0);
                    world.add(make_shared<moving_sphere>(center, center2, 0.0, 1.0, 0.2, sphere_material));

                } else if (choose_material < 0.95) {
                    //metal
                    auto albedo = color::random(0.5, 1.0);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    //glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }

            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

hittable_list earth() {
    auto earth_texture = make_shared<image_texture>("../image/earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);

    auto globe = make_shared<sphere>(point3(0, 0, 0), 2, earth_surface);

    return hittable_list(globe);
}

hittable_list simple_light() {
    hittable_list objects;

    auto pertex = make_shared<noise_texture>(4);
    objects.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertex)));
    objects.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertex)));

    auto light = make_shared<diffuse_light>(color(4, 4, 4));
    objects.add(make_shared<xy_rect>(3, 5, 1, 3, -2, light));
    objects.add(make_shared<sphere>(point3(0, 6, 0), 1, light));
    return objects;
}

hittable_list cornell_box() {
    hittable_list objects;
    auto red = make_shared<lambertian>(color(0.65, 0.05, 0.05));
    auto white = make_shared<lambertian>(color(0.73, 0.73, 0.73));
    auto green = make_shared<lambertian>(color(0.12, 0.45, 0.15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(213, 343, 227, 332, 554, light));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    shared_ptr<hittable> box1 = make_shared<box>(point3(130, 0, 65), point3(165, 330, 165), white);
    box1 = make_shared<rotate>(box1, 15, 1);
    box1 = make_shared<translate>(box1, vec3(265, 0, 295));
    objects.add(box1);

    shared_ptr<hittable> box2 = make_shared<box>(point3(0, 0, 0), point3(165, 165, 165), white);
    box2 = make_shared<rotate>(box2, -18, 1);
    box2 = make_shared<translate>(box2, vec3(130, 0, 65));
    objects.add(box2);
    return objects;
}

bvh_node building_bvh(hittable_list& world, double time0, double time1){
    std::cerr << "\rBuilding BVH Tree..." << std::flush;
    auto start = std::chrono::system_clock::now();
    bvh_node root(world, 0.0, 1.0);
    auto end = std::chrono::system_clock::now();
    std::cerr << "\rBuilding BVH Tree done. Time taken: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "us.\n";

    return root;
}

int main() {
    //image
    auto aspect_ratio = 1920.0 / 1080.0;
    int img_width = 1200;
    int img_height = static_cast<int>(img_width / aspect_ratio);
    int samples_per_pixel;
    const int max_depth = 50;

    point3 lookfrom;
    point3 lookat;
    auto vfov = 40.0;
    auto aperture = 0.0;
    hittable_list world;
    color background(0, 0, 0);

    int choose_scene = 6;

    switch (choose_scene) {
        case 1:
            world = random_scene();
            samples_per_pixel = 200;
            lookfrom = point3(13, 2, 3);
            lookat = point3(0, 0, 0);
            background = color(0.7, 0.8, 1.0);
            vfov = 20.0;
            aperture = 0.1;
            break;
        case 2: {
            lookfrom = point3(13, 2, 3);
            lookat = point3(0, 0, 0);
            vfov = 20.0;
            background = color(0.7, 0.8, 1.0);
            auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
            auto s1 = make_shared<sphere>(point3(0, -10, 0), 10, make_shared<lambertian>(checker));
            auto s2 = make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker));
            world.add(s1);
            world.add(s2);
        }
            break;
        case 3: {
            lookfrom = point3(13, 2, 3);
            lookat = point3(0, 0, 0);
            vfov = 20.0;
            background = color(0.7, 0.8, 1.0);
            auto _texture = make_shared<noise_texture>(4);
            auto s1 = make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(_texture));
            auto s2 = make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(_texture));
            world.add(s1);
            world.add(s2);
        }
            break;
        case 4: {
            background = color(0.7, 0.8, 1.0);
            world = earth();
            lookfrom = point3(13, 2, 3);
            lookat = point3(0, 0, 0);
            vfov = 20.0;
        }
            break;
        case 5: {
            world = simple_light();
            samples_per_pixel = 400;
            background = color(0, 0, 0);

            lookfrom = point3(26, 3, 6);
            lookat = point3(0, 2, 0);
            vfov = 20.0;
        }
            break;
        case 6: {
            world = cornell_box();
            aspect_ratio = 1.0;
            img_width = 600;
            samples_per_pixel = 200;
            background = color(0, 0, 0);
            lookfrom = point3(278, 278, -800);
            lookat = point3(278, 278, 0);
            vfov = 40.0;
        }
            break;
        default:
            break;
    }


    //camera

    vec3 vup(0, 1, 0);
    auto dis_to_focus = 10.0;
    camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dis_to_focus, 0.0, 1.0);

    //time:
    auto bvh_tree = building_bvh(world, 0.0, 1.0);
    auto start = std::chrono::system_clock::now();

    //render

    FILE *fp = fopen("../image/cornellbox_spp200.ppm", "wb");

    (void) fprintf(fp, "P6\n%d %d\n255\n", img_width, img_height);

    for (int j = img_height - 1; j >= 0; --j) {
        std::cerr << "\r" << std::setw(3) << std::setiosflags(std::ios::left)
                  << static_cast<int>((img_height - 1 - j) * 100.0 / img_height) << "%" << " rendered......"
                  << std::flush;
        for (int i = 0; i < img_width; ++i) {
            double r = 0.0, g = 0.0, b = 0.0;
//#pragma omp parallel for reduction( +: r, g, b)
            color pixel_color;
#pragma omp parallel for
            for (int k = 0; k < samples_per_pixel; ++k) {
                auto u = (j + random_double()) / (img_height - 1);
                auto v = (i + random_double()) / (img_width - 1);
                auto temp_color = rayColor(cam.getRay(u, v), background, bvh_tree, max_depth);
//                r += temp_color.x();
//                g += temp_color.y();
//                b += temp_color.z();
                pixel_color += temp_color;
            }
            write_color(fp, pixel_color, samples_per_pixel);

        }
    }
    fclose(fp);
    auto stop = std::chrono::system_clock::now();

    std::cout << "\nRendering done.\n";
    std::cout << "Time taken: " << std::chrono::duration_cast<std::chrono::hours>(stop - start).count() << " hours\n";
    std::cout << "          : " << std::chrono::duration_cast<std::chrono::minutes>(stop - start).count()
              << " minutes\n";
    std::cout << "          : " << std::chrono::duration_cast<std::chrono::seconds>(stop - start).count()
              << " seconds\n";
    return 0;
}