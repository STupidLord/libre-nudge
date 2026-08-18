// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 STupidLord

// Technically this could be a private header,
// since it shouldn't be used alone, but I'm
// not sure on doing that quite yet.

#pragma once

#include "CORE_API.hpp"

#include <filesystem>
#include <vector>

#include "definition.hpp"

namespace core {
CORE_API auto load_province_map(std::filesystem::path directory,
                  const std::vector<province_definition>& provinces)
              -> std::vector<uint16_t>;
}
