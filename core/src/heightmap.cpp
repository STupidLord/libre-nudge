// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 STupidLord

#include "heightmap.hpp"

#include <filesystem>
#include <vector>

#include "definition.hpp"
#include "image.hpp"

namespace fs = std::filesystem;

namespace core {
auto load_heightmap(std::filesystem::path directory,
                    const std::vector<province_definition>& provinces)
     -> std::vector<std::uint8_t> {
    std::vector<std::uint8_t> heights{};

    // heightmap.bmp loads as three channels instead
    // of one because stupid image format bullshit.
    // this hopefully shouldn't cause an issue as
    // long as it is not forgotten when saving data.
    fs::path map_path = fs::weakly_canonical(directory)
                     /= "map/heightmap.bmp";
    std::unique_ptr<image> map = image::load_flipped_image(map_path);

    if (map->get_channels() != 3) return heights;

    heights.assign(map->get_size(), 0);

    int width  = map->get_width();
    int height = map->get_height();

    // This feels wrong... but I think
    // this is the right way to do it
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            pixel p = map->get_pixel(x, y);
            heights[y * width + x] = p.ch1;
        }
    }

    return heights;
}
}
