// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 STupidLord

#include "common.hpp"

#include <vector>
#include <string>
#include <cstddef>
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

namespace fs = std::filesystem;

namespace common {
std::vector<std::string> split(std::string& string,
                                char& delimiter) {
    std::vector<std::string> strings{};
    std::size_t start{0};
    for (std::size_t i = 0; i <= string.length(); i++) {
        if (string[i] == delimiter || i == string.length()) {
            strings.push_back(string.substr(start, i - start));
            start = i + 1;
        }
    }

    return strings;
}

std::vector<std::string> split(const std::string& string,
                                const char& delimiter) {
    std::vector<std::string> strings{};
    std::size_t start{0};
    for (std::size_t i = 0; i <= string.length(); i++) {
        if (string[i] == delimiter || i == string.length()) {
            strings.push_back(string.substr(start, i - start));
            start = i + 1;
        }
    }

    return strings;
}

key_value_pair key_value(std::string& string,
                            char& delimiter) {
    key_value_pair strings{};
    for (std::size_t i = 0; i <= string.length(); i++) {
        if (string[i] == delimiter) {
            strings.key = string.substr(0, i);
            strings.value = string.substr(i+1, string.length()-i+1);
        }
    }

    return strings;
}

key_value_pair key_value(const std::string& string,
                            const char& delimiter) {
    key_value_pair strings{};
    for (std::size_t i = 0; i <= string.length(); i++) {
        if (string[i] == delimiter) {
            strings.key = string.substr(0, i);
            strings.value = string.substr(i+1, string.length()-i+1);
        }
    }

    return strings;
}

std::string between(std::string& string,
                    char& front_delimiter,
                    char& back_delimiter) {
    std::size_t start{0};
    std::size_t end{string.length()-1};

    if (string[start] == front_delimiter && string[end] == back_delimiter)
        return string.substr(start+1, end-1);

    bool front = false;
    bool back  = false;
    for (std::size_t i = 0; i <= string.length(); i++) {
        if (string[i] == front_delimiter) {
            start = i + 1;
            front = true;
        }
        if (string[i] == back_delimiter) {
            end = i;
            back = true;
        }
    }
    
    if (!front || !back) return "";

    return string.substr(start, end-start);
}

std::string between(const std::string& string,
                    const char& front_delimiter,
                    const char& back_delimiter) {
    std::size_t start{0};
    std::size_t end{string.length()-1};

    if (string[start] == front_delimiter && string[end] == back_delimiter)
        return string.substr(start+1, end-1);
    
    bool front = false;
    bool back  = false;
    for (std::size_t i = 0; i <= string.length(); i++) {
        if (string[i] == front_delimiter) {
            start = i + 1;
            front = true;
        }
        if (string[i] == back_delimiter) {
            end = i;
            back = true;
        }
    }
    
    if (!front || !back) return "";

    return string.substr(start, end-start);
}

fs::path get_location() {
#ifdef _WIN32
    std::vector<wchar_t> buffer(MAX_PATH);
    GetModuleFileNameW(NULL, buffer.data(),
                        static_cast<DWORD>(buffer.size()));
    fs::path exePath = buffer.data();
#endif // _WIN32

#ifdef __linux__
    fs::path exePath = fs::canonical("/proc/self/exe");
#endif // __linux__

    return exePath.parent_path().lexically_normal();
}
}
