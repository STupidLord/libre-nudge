// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 STupidLord

#include "buildings.hpp"

#include "parse.hpp"

#include <filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <iterator>

namespace fs = std::filesystem;

namespace core {
namespace {
identifier is_capturable(std::string_view string) {
    if (string == "show_on_map")
        return identifier::count;
    else if (string == "only_costal")
        return identifier::coastal;
    else if (string == "centered")
        return identifier::centered;
    return identifier::unknown;
}

// Technically down one indent by splitting this...
// could be split more, probably.
auto load_buildings_from_tokens(std::vector<internal::token>& tokens)
     -> std::vector<building> {
    std::vector<building> buildings;

    int skip_foward = 0;
    bool in_buildings = false;
    bool in_building = false;
    building current_building{};
    identifier input = identifier::unknown;
    bool input_bool = false;
    int sub_brackets = 0;
    // God I hate this stuff
    // TODO: Clean this up
    for (const internal::token& token : tokens) {
        if (skip_foward > 0) {
            skip_foward--;
            continue;
        }
        if (!in_buildings
            && token.token == internal::token_type::identifier
            && token.value == "buildings") {
            in_buildings = true;
        } else if (in_buildings
                    && token.token == internal::token_type::identifier
                    && !in_building) {
            skip_foward = 2;
            in_building = true;
            current_building.reset_building();
            current_building.type = token.value;
        } else if (in_building
                    && token.token == internal::token_type::identifier) {
            if (input != identifier::unknown) {
                if (token.value == "yes") input_bool = true;
                else input_bool = false;

                switch (input) {
                case identifier::count:
                    current_building.count = std::stoi(token.value);
                    break;
                case identifier::coastal:
                    current_building.coastal = input_bool;
                    break;
                case identifier::centered:
                    current_building.centered = input_bool;
                    break;
                case identifier::no_auto_nudge:
                    current_building.no_auto_nudge = input_bool;
                    break;
                case identifier::unknown:
                    break;
                }

                input = identifier::unknown;
            } else if (identifier id = is_capturable(token.value);
                        id != identifier::unknown) {
                input = id;
            }
        } else if (in_building
                    && token.token == internal::token_type::left_bracket) {
            sub_brackets++;
        } else if (token.token == internal::token_type::right_bracket) {
            if (sub_brackets > 0) sub_brackets--;
            else if (in_building) {
                in_building = false;
                buildings.push_back(current_building);
            } else if (in_buildings) in_buildings = false;
        }
    }

    return buildings;
}
}

// TODO: Maybe make this not strictly load common/buildings? idk
auto load_buildings(std::filesystem::path game_directory)
     -> std::vector<building> {
    std::vector<building> buildings;
    fs::path buildings_path = fs::weakly_canonical(game_directory)
                              /= "common/buildings";

    for (const fs::directory_entry& entry
         : fs::directory_iterator(buildings_path)) {
        const fs::path file = entry.path();
        std::ifstream ifstream(file);
        std::vector<internal::token> tokens = internal::tokenize(ifstream);
        ifstream.close();
        auto _buildings = load_buildings_from_tokens(tokens);

        buildings.insert(buildings.end(),
                         std::make_move_iterator(_buildings.begin()),
                         std::make_move_iterator(_buildings.end()));
    }

    return buildings;
}
}
