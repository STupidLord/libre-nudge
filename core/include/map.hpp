// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 STupidLord

#pragma once

#include "CORE_API.hpp"

#include <vector>
#include <cstdint>
#include <memory>
#include <filesystem>

#include "image.hpp"
#include "definition.hpp"

namespace core {
class map {
private:
    std::vector<province_definition> province_definitions{};
    std::unique_ptr<image>           province_map{};
    std::vector<std::uint16_t>       province_pixels{};
    std::vector<std::uint8_t>        heights{};
public:
    map(std::filesystem::path directory);
};
}
