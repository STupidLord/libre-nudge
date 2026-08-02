// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 STupidLord

#pragma once

#include <vector>
#include <string>
#include <filesystem>

namespace common {
std::vector<std::string> split(std::string& string,
                                char& delimiter);
std::vector<std::string> split(const std::string& string, 
                                const char& delimiter);
struct key_value_pair {
    std::string key;
    std::string value;
};
key_value_pair key_value(std::string& string,
                            char& delimiter);
key_value_pair key_value(const std::string& string, 
                            const char& delimiter);
std::string between(std::string& string,
                    char& front_delimiter,
                    char& back_delimiter);
std::string between(const std::string& string, 
                    const char& front_delimiter,
                    const char& back_delimiter);        
std::filesystem::path get_location();
}
