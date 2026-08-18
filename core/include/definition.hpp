// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 STupidLord

#pragma once

#include <unordered_map>
#include "CORE_API.hpp"

#include <cstdint>
#include <string>
#include <vector>
#include <filesystem>

namespace core {
enum class province_type { land, sea, lake };
struct province_definition {
    uint8_t       r{};
    uint8_t       g{};
    uint8_t       b{};
    province_type type      = province_type::land;
    bool          coastal   = false;
    std::string   terrain   = "unknown";
    int           continent{};

    friend std::string& operator>>(std::string& line,
                                   province_definition& p);

    void clear() {
        *this = province_definition{};
    }
};

CORE_API auto parse_definition_csv(std::filesystem::path directory)
              -> std::vector<province_definition>;
              
CORE_API uint32_t pack_rgb(uint8_t r, uint8_t g, uint8_t b);

// Very cursed wrapping because I couldn't
// decide how to wrap such a long line

//
CORE_API auto rgb_to_province_ids(
                  const std::vector<province_definition>& provinces
              ) -> std::unordered_map<uint32_t, uint16_t>;

}
