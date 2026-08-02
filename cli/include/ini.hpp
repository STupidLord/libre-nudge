// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 STupidLord

#pragma once

#include <filesystem>
#include <expected>

namespace ini {
enum class ini_error { file_not_found, no_fields_set };
struct _config {
    std::filesystem::path game_directory;
    std::filesystem::path user_directory;
};
struct ini {
    _config config;
    bool is_populated = false;
};
std::expected<ini, ini_error> load_ini(std::filesystem::path path);
void save_ini(const std::filesystem::path path, const ini& config);
};
