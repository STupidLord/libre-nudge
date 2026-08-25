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
    std::uint8_t  r{};
    std::uint8_t  g{};
    std::uint8_t  b{};
    province_type type      = province_type::land;
    bool          coastal   = false;
    std::string   terrain   = "unknown";
    int           continent{};

    friend CORE_API std::string& operator>>(std::string& line,
                                            province_definition& p);

    void clear() {
        *this = province_definition{};
    }
};

CORE_API auto parse_definition_csv(std::filesystem::path directory)
              -> std::vector<province_definition>;
              
CORE_API std::uint32_t pack_rgb(std::uint8_t r, std::uint8_t g, std::uint8_t b);

// Very cursed wrapping because I couldn't
// decide how to wrap such a long line

//
CORE_API auto rgb_to_province_ids(
                  const std::vector<province_definition>& provinces
              ) -> std::unordered_map<std::uint32_t, std::uint16_t>;

}
