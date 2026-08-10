// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 STupidLord

#include "buildings.hpp"

#include "parse.hpp"

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>
#include <fstream>
#include <iterator>

namespace fs = std::filesystem;

namespace core {
namespace {
identifier identifier_type_building(std::string_view string) {
    if (string == "show_on_map")
        return identifier::count;
    else if (string == "only_costal")
        return identifier::coastal;
    else if (string == "centered")
        return identifier::centered;
    else if (string == "province_max")
        return identifier::provincial;
    else if (string == "spawn_point")
        return identifier::spawn_point;
    else if (string == "disable_auto_nudging")
        return identifier::no_auto_nudge;
    return identifier::unknown;
}

identifier identifier_type_spawn_point(std::string_view string) {
    if (string == "max")
        return identifier::count;
    else if (string == "only_costal")
        return identifier::coastal;
    else if (string == "type")
        return identifier::provincial;
    else if (string == "disable_auto_nudging")
        return identifier::no_auto_nudge;
    return identifier::unknown;
}

void apply_building_property(building& b,
                             const identifier id,
                             std::string_view value) {
    bool is_yes = (value == "yes");

    switch (id) {
    case identifier::count: b.count = std::stoi(std::string(value)); break;
    case identifier::coastal: b.coastal = is_yes; break;
    case identifier::centered: b.centered = is_yes; break;
    case identifier::provincial: b.provincial = true; break;
    case identifier::spawn_point: b.spawn_point = value; break;
    case identifier::no_auto_nudge: b.no_auto_nudge = is_yes; break;
    default: break;
    }
}

void apply_spawn_point_property(building& b,
                                const identifier id,
                                std::string_view value) {
    bool is_yes = (value == "yes");

    switch (id) {
    case identifier::count: b.count = std::stoi(std::string(value)); break;
    case identifier::coastal: b.coastal = is_yes; break;
    case identifier::provincial: b.provincial = (value == "province"); break;
    case identifier::no_auto_nudge: b.no_auto_nudge = is_yes; break;
    default: break;
    }
}

building parse_building(internal::token_cursor& tokc, std::string_view name) {
    building b{std::string(name)};
    int sub_brakets = 0;

    while (tokc.has_next()) {
        auto tok = tokc.consume();

        if (tok.token == internal::token_type::identifier
         && tokc.block_opening()) {
            sub_brakets++;
            tokc.skip(2);
            continue;
        } else if (tok.token == internal::token_type::identifier
                && identifier_type_building(tok.value) != identifier::unknown) {
            identifier id = identifier_type_building(tok.value);
            apply_building_property(b, id, tokc.peek_ahead(1).value);
            continue;
        } else if (tok.token == internal::token_type::left_bracket) {
            sub_brakets++;
        } else if (tok.token == internal::token_type::right_bracket
                && sub_brakets > 0) {
            sub_brakets--;
            continue;
        } else if (tok.token == internal::token_type::right_bracket) break;
    }

    return b;
}

building parse_spawn_point(internal::token_cursor& tokc, std::string_view name) {
    building b{std::string(name)};
    int sub_brakets = 0;

    while (tokc.has_next()) {
        auto tok = tokc.consume();

        if (tok.token == internal::token_type::identifier
         && identifier_type_spawn_point(tok.value) != identifier::unknown) {
            identifier id = identifier_type_spawn_point(tok.value);
            apply_spawn_point_property(b, id, tokc.peek_ahead(1).value);
            continue;
        } else if (tok.token == internal::token_type::right_bracket) break;
    }

    return b;
}

auto load_buildings_from_tokens(internal::token_cursor& tokc)
     -> std::vector<building> {
    std::vector<building> buildings;

    while (tokc.has_next()) {
        auto tok = tokc.consume();

        if (tok.token == internal::token_type::right_bracket) {
            break;
        } else if (tok.token == internal::token_type::identifier
                && tokc.block_opening()) {
            tokc.skip(2);
            buildings.push_back(parse_building(tokc, tok.value));
        }
    }

    return buildings;
}

auto load_spawn_points_from_tokens(internal::token_cursor& tokc)
     -> std::vector<building> {
    std::vector<building> spawn_points;

    while (tokc.has_next()) {
        auto tok = tokc.consume();

        if (tok.token == internal::token_type::right_bracket) {
            break;
        } else if (tok.token == internal::token_type::identifier
                && tokc.block_opening()) {
            tokc.skip(2);
            spawn_points.push_back(parse_spawn_point(tokc, tok.value));
        }
    }

    return spawn_points;
}

auto load_buildings_file_from_tokens(std::vector<internal::token>& tokens)
     -> std::vector<building> {
    std::vector<building> buildings;
    internal::token_cursor tokc{tokens};

    while (tokc.has_next()) {
        auto tok = tokc.consume();

        if (tok.token == internal::token_type::identifier
         && tok.value == "buildings") {
            auto _buildings = load_buildings_from_tokens(tokc);
            buildings.insert(buildings.end(),
                             std::make_move_iterator(_buildings.begin()),
                             std::make_move_iterator(_buildings.end()));;
        } else if (tok.token == internal::token_type::identifier
                && tok.value == "spawn_points") {
            auto _buildings = load_spawn_points_from_tokens(tokc);
            buildings.insert(buildings.end(),
                             std::make_move_iterator(_buildings.begin()),
                             std::make_move_iterator(_buildings.end()));;
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
        auto _buildings = load_buildings_file_from_tokens(tokens);

        buildings.insert(buildings.end(),
                         std::make_move_iterator(_buildings.begin()),
                         std::make_move_iterator(_buildings.end()));
    }

    return buildings;
}
}
