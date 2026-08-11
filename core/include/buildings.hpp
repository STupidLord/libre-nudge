// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 STupidLord

#pragma once

#ifdef _WIN32
#include "windowsdll.hpp"
#endif // _WIN32

#include <vector>
#include <filesystem>

namespace core {
enum class identifier {
    unknown,

    count,
    coastal,
    centered,
    no_auto_nudge,
    provincial,
    spawn_point
};

struct building { // Spawn points count as buildings :hmmdevious:
    std::string type          = "unknown";
    int         count         = 0;
    bool        coastal       = false;
    bool        centered      = false;
    bool        no_auto_nudge = false;
    bool        provincial    = false;
    std::string spawn_point   = "none";
};

CORE_API auto load_buildings(std::filesystem::path game_directory)
              -> std::vector<building>;

struct buildings_line {
    int      state;
    building model_building;
    // These 4 have 2 decimal didgets
    int      model_x;
    int      model_y;
    int      model_z;
    int      model_rotation;
    //
    int      sea_province;
};
};
