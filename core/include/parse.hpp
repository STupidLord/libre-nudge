// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 STupidLord

// Technically this could be a private header,
// since it shouldn't be used alone, but I'm
// not sure on doing that quite yet.

#pragma once

#ifdef _WIN32
#include "windowsdll.hpp"
#endif // _WIN32

#include <string>
#include <vector>
#include <fstream>

namespace core {
namespace internal {
    enum class token_type { identifier, equals, left_bracket, right_bracket };
    struct token {
        token_type token;
        std::string value;
    };

    CORE_API std::vector<token> tokenize(std::ifstream& file);
    // Mostly for visualizing tokenization
    CORE_API void reconstruct_file(std::vector<token>& tokens,
                                   std::ofstream& file);
}
}
