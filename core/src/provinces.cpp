// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 STupidLord

#include "provinces.hpp"

#include <cstdint>
#include <filesystem>
#include <memory>
#include <vector>

#include "definition.hpp"
#include "image.hpp"

namespace fs = std::filesystem;

namespace core {
auto load_province_map(std::filesystem::path directory,
                       const std::vector<province_definition>& provinces)
     -> std::vector<uint16_t> {
    std::vector<uint16_t> labels{};

    fs::path map_path = fs::weakly_canonical(directory)
                     /= "map/provinces.bmp";
    std::unique_ptr<image> map = image::load_flipped_image(map_path);

    if (map->get_channels() != 3) return labels;

    labels.assign(map->get_size(), 0);

    auto id_map = rgb_to_province_ids(provinces);

    int width  = map->get_width();
    int height = map->get_height();

    // This feels wrong... but I think
    // this is the right way to do it
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            pixel p = map->get_pixel(x, y);
            uint32_t rgb = pack_rgb(p.ch1, p.ch2, p.ch3);
            if (auto id = id_map.find(rgb); id != id_map.end()) {
                labels[y * width + x] = id->second;
            }
        }
    }

    return labels;
}
}
