// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 STupidLord

#include <cstdlib>
#include <filesystem>
#include <print>

#include "version.hpp"
#include "common.hpp"
#include "arg.hpp"
#include "ini.hpp"

#include <buildings.hpp>
#include <libre-nudge/version.hpp>

int main(int argc, char* argv[]) {
    std::filesystem::path config_path = common::get_location() /= "config.ini";
    auto config = ini::load_ini(config_path);

    // Error handling if I actually care enough to set it up
    if (!config.has_value()) {
        switch (config.error()) {
        case ini::ini_error::file_not_found:
            break;
        case ini::ini_error::no_fields_set:
            break;
        }
        config = ini::ini{};
    }

    // Not sure if this is actually faster or not :hmmdevious:
    const std::vector<std::string_view> args = arg::args_from_argv(argc, argv);
    arg::argv_result args_result = arg::parse_args(args);
    if (int rv = arg::handle_error(args_result)) return rv;

    // TODO: Move print handling to own function
    if (args_result.flags.print_help) {}
    if (args_result.flags.print_version) {
        std::println(
            LN_TMV_STR "\n"
            LN_CR_STR  "\n"
            LN_GPL_STR "\n",
            LN_V_MAJOR, LN_V_MINOR, LN_V_PATCH
        );
        std::println(
            LNB_TMV_STR "\n"
            LNB_CR_STR  "\n"
            LNB_GPL_STR,
            LNB_V_MAJOR, LNB_V_MINOR, LNB_V_PATCH
        );
        return EXIT_SUCCESS;
    }

    // TODO: Move required handling to own function
    if (config->config.game_directory.empty()
        && args_result.values.game_directory->empty()) {
        std::println(stderr,
                    "No game directory value provided "
                    "in config or as a argument");
        return EXIT_FAILURE;
    } else if (args_result.values.game_directory.has_value())
        config->config.game_directory
        = args_result.values.game_directory.value();

    if (config->config.user_directory.empty()
        && args_result.values.user_directory->empty()) {
        std::println(stderr,
                    "No user directory value provided "
                    "in config or as a argument");
        return EXIT_FAILURE;
    } else if (args_result.values.user_directory.has_value())
        config->config.user_directory
        = args_result.values.user_directory.value();

    // core::load_buildings(config->config.game_directory);

    auto buildings = core::load_buildings(config->config.game_directory);
    for (auto& building : buildings) {
        std::println("Building: {}\n"
                     " Count: {}\n"
                     " Coastal: {}\n"
                     " Centered: {}\n"
                     " No auto nudge: {}",
                     building.type,
                     building.count,
                     building.coastal,
                     building.centered,
                     building.no_auto_nudge);
    }

    // std::ifstream ifile(ifilep);
    // std::filesystem::path ofilep = std::filesystem::weakly_canonical(config->config.user_directory) /= "reconstructed.txt";
    // std::println("ofile: {}", ofilep.string());
    // std::ofstream ofile(ofilep);
    // auto tokens = core::internal::tokenize(ifile);
    // core::internal::reconstruct_file(tokens, ofile);

    ini::save_ini(config_path, *config);

    return EXIT_SUCCESS;
}
