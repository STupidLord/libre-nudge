// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 STupidLord

#include "image.hpp"

#include <cstddef>
#include <memory>
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace core {
image::~image() { stbi_image_free(this->data); }

std::unique_ptr<image> image::load_image(std::filesystem::path path) {
    int x;
    int y;
    int n;

    std::string path_str = path.string();

    unsigned char* data = stbi_load(path_str.c_str(), &x, &y, &n, 0);

    if (data == NULL) return nullptr;

    switch (n) {
    case 4:  return std::make_unique<rgba_image>(x, y, data);
    case 3:  return std::make_unique<rgb_image>(x, y, data);
    case 2:  return std::make_unique<gsa_image>(x, y, data);
    case 1:  return std::make_unique<gs_image>(x, y, data);
    default: return nullptr;
    }
}

std::unique_ptr<image> image::load_flipped_image(std::filesystem::path path) {
    int x;
    int y;
    int n;

    std::string path_str = path.string();

    stbi_set_flip_vertically_on_load(1); // Load bottom left origin
    unsigned char* data = stbi_load(path_str.c_str(), &x, &y, &n, 0);
    stbi_set_flip_vertically_on_load(0); // Not sure if needed

    if (data == NULL) return nullptr;

    std::unique_ptr<image> image;

    switch (n) {
    case 4:  return std::make_unique<rgba_image>(x, y, data);
    case 3:  return std::make_unique<rgb_image>(x, y, data);
    case 2:  return std::make_unique<gsa_image>(x, y, data);
    case 1:  return std::make_unique<gs_image>(x, y, data);
    default: return nullptr;
    }
}

pixel gs_image::get_pixel(int x, int y) {
    // Not sure if this should be returned as pure black pixel at 0, 0 or not
    if (x > this->get_width() || y > this->get_height()) return pixel();
    int index = (y * this->get_width() + x);
    return pixel(x, y,
                 this->get_data(index),
                 255,
                 0,
                 0);
}

pixel gsa_image::get_pixel(int x, int y) {
    if (x > this->get_width() || y > this->get_height()) return pixel();
    int index = (y * this->get_width() + x) * 2;
    return pixel(x, y,
                 this->get_data(index),
                 this->get_data(index+1),
                 0,
                 0);
}

pixel rgb_image::get_pixel(int x, int y) {
    if (x > this->get_width() || y > this->get_height()) return pixel();
    int index = (y * this->get_width() + x) * 3;
    return pixel(x, y,
                 this->get_data(index),
                 this->get_data(index+1),
                 this->get_data(index+2),
                 255);
}

pixel rgba_image::get_pixel(int x, int y) {
    if (x > this->get_width() || y > this->get_height()) return pixel();
    int index = (y * this->get_width() + x) * 4;
    return pixel(x, y,
                 this->get_data(index),
                 this->get_data(index+1),
                 this->get_data(index+2),
                 this->get_data(index+3));
}
}
