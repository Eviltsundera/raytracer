#pragma once

#include <image.h>
#include <camera_options.h>
#include <render_options.h>
#include <string>
#include <parser.h>
#include <optional>
#include <helpers.h>

#include "Traycer.h"

Image Render(const std::string& filename_obj, const CameraOptions& camera_options,
             const RenderOptions& render_options) {
    Parser parser(filename_obj);
    Objects objects = parser.RunParse();
    double width = camera_options.screen_width, height = camera_options.screen_height;
    Image image(width, height);
    double fov = camera_options.fov;
    // TODO add rotate matrix and centre of pixel
    std::vector<std::vector<Pixel>> res_img(height, std::vector<Pixel>(width));

    Point from = {camera_options.look_from[0], camera_options.look_from[1],
                  camera_options.look_from[2]};
    Point to = {camera_options.look_to[0], camera_options.look_to[1], camera_options.look_to[2]};

    Point forward = (from - to).Normalize();
    Point tmp = Point(0, 1, 0);
    if (tmp.IsParallel(forward)) {
        tmp = Point(0, 0, -1);
    }
    Point right = VectorMul(tmp, forward).Normalize();
    Point up = VectorMul(forward, right).Normalize();
    Point e1 = Pixel{right.coord_x_, up.coord_x_, forward.coord_x_}.Normalize(),
          e2 = Pixel{right.coord_y_, up.coord_y_, forward.coord_y_}.Normalize(),
          e3 = Pixel{right.coord_z_, up.coord_z_, forward.coord_z_}.Normalize();
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            if (i == 50 && j == 50) {
                std::cout << i;
            }
            double px = (2.0 * (i + 0.5) / width - 1) * tan(fov / 2.0) * width / height;
            double py = -(2.0 * (j + 0.5) / height - 1) * tan(fov / 2.0);
            Point v = Point{px, py, -1}.Normalize();
            Point direction = Point{
                v * e1,
                v * e2,
                v * e3
            }.Normalize();
            Pixel pp = TraceRay(from,
                                direction,
                                objects,
                                render_options.depth);
            res_img[j][i] = pp;
        }
    }
    double C = 0;
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            C = std::max(C, res_img[j][i].coord_x_);
            C = std::max(C, res_img[j][i].coord_y_);
            C = std::max(C, res_img[j][i].coord_z_);
        }
    }
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            Pixel v1 = Pixel{1, 1, 1} + res_img[j][i] / C / C;
            Pixel v2 = DivideChannel(res_img[j][i], Pixel{1, 1, 1} + res_img[j][i]);
            res_img[j][i] = Power(MulChannel(v1, v2), 1 / 2.2);
        }
    }
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            Pixel pp = res_img[j][i];
            RGB pix = {int(pp.coord_x_ * 255), int(pp.coord_y_ * 255), int(pp.coord_z_ * 255)};
            image.SetPixel(pix, j, i);
        }
    }
    return image;
}