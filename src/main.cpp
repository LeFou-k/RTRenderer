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
#include "../include/box.h"
#include "../include/rotate.h"
#include "../include/translate.h"
#include "../include/constant_medium.h"
#include "../include/hittable_pdf.h"
#include "../include/mixture_pdf.h"
#include "../include/flip_face.h"


#include <omp.h>
#include <iostream>
#include <cmath>
#include <fstream>
#include <chrono>
#include <iomanip>


//传递一个hittable_list，返回光线上最近的位置t


color rayColor(Ray ray, const color &background, const hittable &world, int depth, shared_ptr<hittable> light) {
    if (depth <= 0) return {0, 0, 0};

    hit_record rec;
    if (!world.hit(ray, 0.001, infinity, rec))
        return background;

    color emitted = rec.mat_ptr->emitted(ray, rec, rec.u, rec.v, rec.p);
    //pdf to calculate
    double pdf_val;
    //if scattered false, which means hit the light:
    scatter_record srec;
    if (!rec.mat_ptr->scatter(ray, rec, srec))
        return emitted;

    if (srec.is_specular) {
        return srec.attenuation * rayColor(srec.specular_ray, background, world, depth - 1, light);
    }

    auto light_ptr = make_shared<hittable_pdf>(light, rec.p);
    mixture_pdf mixed_pdf(light_ptr, srec.pdf_ptr);


    Ray scattered = Ray(rec.p, mixed_pdf.generate(), ray.time());
    pdf_val = mixed_pdf.value(scattered.direction());

    //scatter_pdf: the pdf of scattering

    return emitted + srec.attenuation * rec.mat_ptr->scatter_pdf(ray, rec, scattered) *
                     rayColor(scattered, background, world, depth - 1, light) / pdf_val;
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
    objects.add(make_shared<flip_face>(make_shared<xz_rect>(213, 343, 227, 332, 554, light)));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    shared_ptr<material> aluminum = make_shared<metal>(color(0.8, 0.85, 0.88), 0.0);
    shared_ptr<hittable> box1 = make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
    box1 = make_shared<rotate>(box1, 15, 1);
    box1 = make_shared<translate>(box1, vec3(265, 0, 295));
    objects.add(box1);

//    shared_ptr<hittable> box2 = make_shared<box>(point3(0, 0, 0), point3(165, 165, 165), white);
//    box2 = make_shared<rotate>(box2, -18, 1);
//    box2 = make_shared<translate>(box2, vec3(130, 0, 65));
//    objects.add(box2);


    auto glass = make_shared<dielectric>(1.5);
    objects.add(make_shared<sphere>(point3(190, 90, 190), 90, glass));

    return objects;
}

hittable_list cornell_smoke() {
    hittable_list objects;
    auto red = make_shared<lambertian>(color(0.65, 0.05, 0.05));
    auto white = make_shared<lambertian>(color(0.73, 0.73, 0.73));
    auto green = make_shared<lambertian>(color(0.12, 0.45, 0.15));
    auto light = make_shared<diffuse_light>(color(7, 7, 7));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(113, 443, 127, 432, 554, light));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    shared_ptr<hittable> box1 = make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
    box1 = make_shared<rotate>(box1, 15, 1);
    box1 = make_shared<translate>(box1, vec3(265, 0, 295));

    shared_ptr<hittable> box2 = make_shared<box>(point3(0, 0, 0), point3(165, 165, 165), white);
    box2 = make_shared<rotate>(box2, -18, 1);
    box2 = make_shared<translate>(box2, vec3(130, 0, 65));


    objects.add(make_shared<constant_medium>(box1, 0.01, color(0, 0, 0)));
    objects.add(make_shared<constant_medium>(box2, 0.01, color(1, 1, 1)));

    return objects;
}

hittable_list final_scene() {
    hittable_list boxes1;
    auto ground = make_shared<lambertian>(color(0.48, 0.83, 0.53));

    const int boxes_per_side = 20;
    //ground boxes
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i * w;
            auto z0 = -1000.0 + j * w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_double(1, 101);
            auto z1 = z0 + w;

            boxes1.add(make_shared<box>(point3(x0, y0, z0), point3(x1, y1, z1), ground));
        }
    }

    hittable_list objects;
    //add ground boxes into bvh_nodes
    objects.add(make_shared<bvh_node>(boxes1, 0, 1));
    //add light
    auto light = make_shared<diffuse_light>(color(7, 7, 7));
    objects.add(make_shared<xz_rect>(123, 423, 147, 412, 554, light));
    //add moving sphere
    auto center1 = point3(400, 400, 200);
    auto center2 = center1 + vec3(30, 0, 0);
    auto moving_sphere_material = make_shared<lambertian>(color(0.7, 0.3, 0.1));
    objects.add(make_shared<moving_sphere>(center1, center2, 0, 1, 50, moving_sphere_material));
    //add dielectric ball
    objects.add(make_shared<sphere>(point3(260, 150, 45), 50, make_shared<dielectric>(1.5)));
    //add metal grey ball
    objects.add(make_shared<sphere>(
            point3(0, 150, 145), 50, make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)
    ));

    //add blue dielectric ball
    //dielectric boundary
    auto boundary = make_shared<sphere>(point3(360, 150, 145), 70, make_shared<dielectric>(1.5));
    objects.add(boundary);

    objects.add(make_shared<constant_medium>(boundary, 0.2, color(1.0, 0.8, 0.8)));
    //big ball boundary
    boundary = make_shared<sphere>(point3(0, 0, 0), 5000, make_shared<dielectric>(1.5));
    //add constant_medium in the whole big dielectric sphere
    objects.add(make_shared<constant_medium>(boundary, .0001, color(1, 1, 1)));

    //add earth to scene
    auto emat = make_shared<lambertian>(make_shared<image_texture>("../image/earthmap.jpg"));
    objects.add(make_shared<sphere>(point3(400, 200, 400), 100, emat));
    //add noise_texture sphere to scene
    auto pertext = make_shared<noise_texture>(0.1);
    objects.add(make_shared<sphere>(point3(220, 280, 300), 80, make_shared<lambertian>(pertext)));

    hittable_list boxes2;
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    //add small balls to box2
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<sphere>(point3::random(0, 165), 10, white));
    }
    //rotate box2
    objects.add(make_shared<translate>(make_shared<rotate>(make_shared<bvh_node>(boxes2, 0.0, 1.0), 15, 1),
                                       vec3(-100, 270, 395)));

    return objects;
}

bvh_node building_bvh(hittable_list &world, double time0, double time1) {
    std::cerr << "\rBuilding BVH Tree..." << std::flush;
    auto start = std::chrono::system_clock::now();
    bvh_node root(world, 0.0, 1.0);
    auto end = std::chrono::system_clock::now();
    std::cerr << "\rBuilding BVH Tree done. Time taken: "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "us.\n";

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

//    shared_ptr<hittable> lights = make_shared<flip_face>(
//            make_shared<xz_rect>(213, 343, 227, 332, 554, shared_ptr<material>()));
    auto lights = make_shared<hittable_list>();
    lights->add(make_shared<sphere>(point3(190, 90, 190), 90, make_shared<material>()));
    lights->add(make_shared<xz_rect>(213, 343, 227, 332, 554, make_shared<material>()));

    int choose_scene = 6;
    switch (choose_scene) {
        case 1:
            world = random_scene();
            samples_per_pixel = 200;
            img_width = 1200;
            img_height = static_cast<int>(img_width / aspect_ratio);
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
            img_height = static_cast<int>(img_width / aspect_ratio);
            samples_per_pixel = 10000;
            background = color(0, 0, 0);
            lookfrom = point3(278, 278, -800);
            lookat = point3(278, 278, 0);
            vfov = 40.0;
        }
            break;
        case 7: {
            world = cornell_smoke();
            aspect_ratio = 1.0;
            img_width = 600;
            img_height = static_cast<int>(img_width / aspect_ratio);
            samples_per_pixel = 100;
            lookfrom = point3(278, 278, -800);
            lookat = point3(278, 278, 0);
            vfov = 40.0;
        }
            break;
        case 8: {
            world = final_scene();
            aspect_ratio = 1.0;
            img_width = 800;
            img_height = static_cast<int>(img_width / aspect_ratio);
            samples_per_pixel = 20000;
            background = color(0, 0, 0);
            lookfrom = point3(478, 278, -600);
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

    FILE *fp = fopen("../image/CornellBox_final_spp10000.ppm", "wb");

    (void) fprintf(fp, "P6\n%d %d\n255\n", img_width, img_height);

    for (int j = img_height - 1; j >= 0; --j) {
        std::cerr << "\r" << std::setw(3) << std::setiosflags(std::ios::left)
                  << static_cast<int>((img_height - 1 - j) * 100.0 / img_height) << "%" << " rendered......"
                  << std::flush;
        for (int i = 0; i < img_width; ++i) {
            double r = 0.0, g = 0.0, b = 0.0;

#pragma omp parallel for reduction(+: r, g, b) default(none) private(i, j) shared(samples_per_pixel, img_height, img_width, cam, background, bvh_tree, maxdepth, lights)
            for (int k = 0; k < samples_per_pixel; ++k) {
                auto u = (j + random_double()) / (img_height - 1);
                auto v = (i + random_double()) / (img_width - 1);
                auto temp_color = rayColor(cam.getRay(u, v), background, bvh_tree, max_depth, lights);
                r += temp_color.x();
                g += temp_color.y();
                b += temp_color.z();
            }
            write_color(fp, color(r, g, b), samples_per_pixel);

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