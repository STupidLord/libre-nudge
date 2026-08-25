// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 STupidLord

#include "image.hpp"

#include <array>
#include <cstddef>
#include <fstream>
#include <ios>
#include <memory>
#include <filesystem>
#include <fstream>
#include <print>
#include <cmath>

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
    case 4:  return std::make_unique<rgba_image>(x, y, data, 4);
    case 3:  return std::make_unique<rgb_image>(x, y, data, 3);
    case 2:  return std::make_unique<gsa_image>(x, y, data, 2);
    case 1:  return std::make_unique<gs_image>(x, y, data, 1);
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

    switch (n) {
    case 4:  return std::make_unique<rgba_image>(x, y, data, 4);
    case 3:  return std::make_unique<rgb_image>(x, y, data, 3);
    case 2:  return std::make_unique<gsa_image>(x, y, data, 2);
    case 1:  return std::make_unique<gs_image>(x, y, data, 1);
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

namespace bitmap {
std::ifstream& operator>>(std::ifstream& is, bitmap_rgb_quad& c) {
    is >> c.r >> c.g >> c.b >> c.a;
    return is;
}

// TODO: error handling, like a lot of error handling
//
// TODO: add more file agnostic parsing, currently
// only handles 8-bit indexed
//
// TODO: part of the prior, but implement coverage of
// the full BMP format
void read_bitmap(std::filesystem::path path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) return;

    bitmap_file_header header;
    file.read(reinterpret_cast<char*>(&header), sizeof(bitmap_file_header));
    if (header.signature != std::array{'B', 'M'}) return;

    bitmap_header_type header_type;
    file.read(reinterpret_cast<char*>(&header_type), sizeof(bitmap_header_type));
    if (header_type != 40) return;

    std::streamsize read{file.gcount()};
    file.seekg(-read, std::ios_base::cur);
    bitmap_info_header info_header;
    file.read(reinterpret_cast<char*>(&info_header), sizeof(bitmap_info_header));

    std::size_t table_size{0};
    if (info_header.bit_count == 8) table_size = 256;
 
    std::vector<bitmap_rgb_quad> colors(table_size);
    for (std::size_t i = 0; i < table_size; i++)
        file.read(reinterpret_cast<char*>(&colors[i]), sizeof(bitmap_rgb_quad));

    file.close();

    std::println("Test: {}", colors[255].r);
}
}
}
