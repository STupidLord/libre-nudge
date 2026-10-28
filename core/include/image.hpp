// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 STupidLord

// Technically this could be a private header,
// since it shouldn't be used alone, but I'm
// not sure on doing that quite yet.

#pragma once

#include "CORE_API.hpp"

#include <cstdint>
#include <memory>
#include <filesystem>
#include <array>
#include <cstddef>
#include <fstream>

// TODO: Easier way to select a pixel than manually typing y * width + x
// Maybe doesn't belong here, but this is the general image related place

namespace core {
struct pixel {
    int          x  {};
    int          y  {};
    std::uint8_t ch1{};
    std::uint8_t ch2{};
    std::uint8_t ch3{};
    std::uint8_t ch4{};
};

class CORE_API image {
private:
    int            width   {};
    int            height  {};
    unsigned char* pixels  {};
    std::uint8_t   channels{};

public:
    image(int width, int height, unsigned char* pixels, std::uint8_t c)
        : width(width), height(height), pixels(pixels), channels(c) {};
    virtual ~image(); // Free image data with stbi_image_free()

    int            get_width()           { return this->width; };
    int            get_height()          { return this->height; };
    int            get_size()            { return this->width * this->height; };
    unsigned char* get_pixels()          { return this->pixels; };
    unsigned char  get_pixels(int index) { return this->pixels[index]; };
    std::uint8_t   get_channels()        { return this->channels; };

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

// Indexed BMPs
namespace bitmap {

// Force no alignment padding
#pragma pack(push, 1)

// I don't need to account for little and big endian
// differences since BMP is little endian and so are
// modern operating systems. Does this mean it's 
// technically a bad implementation? Maybe. Do I care?
// No.

// This implementation also strays away from the
// offical terminology as used by Microsoft...
// because I hate it. I'm not calling it "biSize" just
// because you say that's the offical name of that
// collection of bytes in the header.

const std::array<char, 2> signature{'B', 'M'};
// BITMAPFILEHEADER
struct bitmap_file_header {
    std::array<char, 2> signature;
    std::uint32_t       size;
    // "Reserved"s don't really matter, they don't do
    // anything, but might as well parse them properly.
    std::uint16_t       reserved_1;
    std::uint16_t       reserved_2;
    std::uint32_t       offset;
};
typedef std::uint32_t bitmap_header_type;
// BITMAPINFOHEADER
// TODO: add notes for each property
struct bitmap_info_header {
    std::uint32_t size;
    std::int32_t  width;
    std::int32_t  height;
    std::uint16_t planes;
    std::uint16_t bit_count;
    std::uint32_t compression;
    std::uint32_t image_size;
    std::int32_t  x_pixels_per_meter;
    std::int32_t  y_pixels_per_meter;
    std::uint32_t colors_used;
    std::uint32_t important_colors;
};
struct bitmap_rgb_quad {
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;
    // Alpha only exists under certain headers
    // most headers simply use this slot for
    // 4-byte padding
    std::uint8_t a;
};

#pragma pack(pop)

enum bitmap_flip_vertically {
    NO = 0,
    YES = 1,
    AUTOMATIC = 2
};

// Cursed formatting my beloved
CORE_API void read_bitmap(std::filesystem::path path,
                          bitmap_flip_vertically flip_vertically
                              = bitmap_flip_vertically::AUTOMATIC);
}
}
