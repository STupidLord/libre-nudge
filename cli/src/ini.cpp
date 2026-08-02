// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 STupidLord

#include "ini.hpp"

#include <filesystem>
#include <fstream>
#include <expected>
#include <string>

#include "common.hpp"

namespace fs = std::filesystem;

namespace ini {
std::expected<ini, ini_error> load_ini(fs::path path) {
    fs::path _path = fs::weakly_canonical(path);
    if (!fs::exists(_path)) return std::unexpected(ini_error::file_not_found);
    std::ifstream config_file(_path);
    
    ini config{};

    std::string line{};
    std::string section{};
    while (std::getline(config_file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();

        // I know this is probably inefficient but idc
        if (auto _section = common::between(line, '[', ']'); !_section.empty()){
            section = _section;
            continue;
        } else if (auto kv = common::key_value(line, '=');
            !kv.key.empty() && !kv.value.empty() && !section.empty()) {
            if (section == "config") {
                config.is_populated = true;
                if (kv.key == "game_directory") {
                    config.config.game_directory = fs::path(kv.value);
                } else if (kv.key == "user_directory") {
                    config.config.user_directory = fs::path(kv.value);
                }
            }
        }
    }

    config_file.close();
    if (!config.is_populated) return std::unexpected(ini_error::no_fields_set);
    return config;
}

void save_ini(const std::filesystem::path path, const ini& config) {
    fs::path _path = fs::weakly_canonical(path);
    std::ofstream config_file(_path);

    // Probably isn't best practice to rewrite the whole
    // file every time... but I really don't care that much
    config_file << "[config]\n"
                << "game_directory="
                   << config.config.game_directory.string() << "\n"
                << "user_directory="
                   << config.config.user_directory.string() << "\n";

    config_file.close();
}
}
