// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 STupidLord

#include "definition.hpp"

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <charconv>
#include <iterator>
#include <regex>

namespace fs = std::filesystem;

namespace core {
namespace {
province_type parse_province_type(const std::string& str) {
    if (str == "land") return province_type::land;
    else if (str == "sea") return province_type::sea;
    else return province_type::lake;
} 
}

std::string& operator>>(std::string& line,
                        province_definition& p) {
    std::vector<std::string> properties{};
    std::regex               separator("\\;");
    std::copy(
        std::sregex_token_iterator(line.begin(), line.end(), separator, -1),
        std::sregex_token_iterator(),
        std::back_inserter(properties)
    );

    if (properties.size() == 8) {
        std::from_chars(properties[1].data(),
                        properties[1].data() + properties[1].size(),
                        p.r);
        std::from_chars(properties[2].data(),
                        properties[2].data() + properties[1].size(),
                        p.g);
        std::from_chars(properties[3].data(),
                        properties[3].data() + properties[1].size(),
                        p.b);

        p.type      = parse_province_type(properties[4]);
        p.coastal   = (properties[5] == "true");
        p.terrain   = properties[6];
        p.continent = std::stoi(properties[7]);
    }

    return line;
}

auto parse_definition_csv(fs::path directory)
     -> std::vector<province_definition> {
    std::vector<province_definition> provinces{};
    province_definition province{};
    
    fs::path definitions_path = fs::weakly_canonical(directory)
                             /= "map/definition.csv";
    std::ifstream file{definitions_path};

    std::string line{};
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();

        line >> province;
        provinces.push_back(province);
        province.clear();
    }

    return provinces;
}
}
