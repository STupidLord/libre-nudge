// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 STupidLord

#pragma once

#ifdef _WIN32
#include "windowsdll.hpp"
#endif // _WIN32

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
}
