// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 STupidLord

#pragma once

#include "CORE_API.hpp"

#include <filesystem>
#include <vector>

#include "definition.hpp"

namespace core {
CORE_API auto load_heightmap(std::filesystem::path directory,
                  const std::vector<province_definition>& provinces)
              -> std::vector<std::uint8_t>;
}
