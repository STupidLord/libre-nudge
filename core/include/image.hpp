// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 STupidLord

// Technically this could be a private header,
// since it shouldn't be used alone, but I'm
// not sure on doing that quite yet.

#pragma once

#ifdef _WIN32
#include "windowsdll.hpp"
#endif // _WIN32

#include <cstdint>
#include <memory>
#include <filesystem>

namespace core {
struct pixel {
    int     x  {};
    int     y  {};
    uint8_t ch1{};
    uint8_t ch2{};
    uint8_t ch3{};
    uint8_t ch4{};
};

class CORE_API image {
private:
    int            width {};
    int            height{};
    unsigned char* data  {};

public:
    image(int x, int y, unsigned char* d)
        : width(x), height(y), data(d) {};
    virtual ~image(); // Free image data with stbi_image_free()

    int            get_width()         { return this->width; };
    int            get_height()        { return this->height; };
    int            get_size()          { return this->width * this->height; };
    unsigned char* get_data()          { return this->data; };
    unsigned char  get_data(int index) { return this->data[index]; };

    virtual pixel get_pixel(int x, int y) = 0;

    static std::unique_ptr<image> load_image(std::filesystem::path path);
    static std::unique_ptr<image> load_flipped_image(std::filesystem::path path);
};

class CORE_API gs_image : public image {
public:
    using image::image;

    pixel get_pixel(int x, int y) override;
};

class CORE_API gsa_image : public image {
public:
    using image::image;

    pixel get_pixel(int x, int y) override;
};

class CORE_API rgb_image : public image {
public:
    using image::image;

    pixel get_pixel(int x, int y) override;
};

class CORE_API rgba_image : public image {
public:
    using image::image;

    pixel get_pixel(int x, int y) override;
};
}
