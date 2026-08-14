// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 STupidLord

#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <memory>
#include <print>
#include <ranges>
#include <utility>

#include "version.hpp"
#include "common.hpp"
#include "arg.hpp"
#include "ini.hpp"

#include <buildings.hpp>
#include <definition.hpp>
#include <image.hpp>
#include <provinces.hpp>
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

    // Moved in front of error handling because I forgor
    // TODO: Move print handling to own function
    if (args_result.flags.print_help) {
        return EXIT_SUCCESS;
    }
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

    if (int rv = arg::handle_error(args_result)) return rv;

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

    // auto buildings = core::load_buildings(config->config.game_directory);
    // for (auto& building : buildings) {
    //     std::println("Building: {}\n"
    //                  " Count: {}\n"
    //                  " Coastal: {}\n"
    //                  " Centered: {}\n"
    //                  " No auto nudge: {}\n"
    //                  " Provincial: {}\n"
    //                  " Spawn point: {}",
    //                  building.type,
    //                  building.count,
    //                  building.coastal,
    //                  building.centered,
    //                  building.no_auto_nudge,
    //                  building.provincial,
    //                  building.spawn_point);
    // }

    auto provinces = core::parse_definition_csv(config->config.game_directory);
    // for (auto&& [index, p] : std::views::enumerate(provinces)) {
    //     std::println("{}: rgb {},{},{}; type {};"
    //                  " coastal {}; terrain {}; continent {}",
    //                  index,
    //                  p.r,
    //                  p.g,
    //                  p.b,
    //                  std::to_underlying(p.type),
    //                  p.coastal,
    //                  p.terrain,
    //                  p.continent);
    // }
    auto provinces_map = core::load_province_map(config->config.game_directory,
                                                 provinces);
    // uint16_t id = provinces_map.at(1018 * 5632 + 2825);
    // uint16_t id = provinces_map.at(1167 * 5632 + 3359);
    // auto p = provinces.at(id);
    int forest_count{};
    for (auto [pos, pixel] : std::views::enumerate(provinces_map)) {
        auto p = provinces.at(pixel);
        if (p.terrain == "forest") {
            forest_count++;
            // std::println("{} {},{}: rgb {},{},{}; type {};"
            //              " coastal {}; terrain {}; continent {}",
            //              pos,
            //              pos % 5632,
            //              pos / 5632,
            //              p.r,
            //              p.g,
            //              p.b,
            //              std::to_underlying(p.type),
            //              p.coastal,
            //              p.terrain,
            //              p.continent);
        }
    }
    std::println("Amount of pixels of forest: {}", forest_count);
            
    // std::filesystem::path image_path("D:/dev/projects/libre-nudge/examples/pixel_test.bmp");
    // std::unique_ptr<core::image> image = core::image::load_flipped_image(image_path);

    // core::pixel p1 = image->get_pixel(0, 99);
    // core::pixel p2 = image->get_pixel(99, 0);
    // core::pixel p3 = image->get_pixel(0, 30);

    // std::println("Pixel 1: {}, {}, {}", p1.ch1, p1.ch2, p1.ch3);
    // std::println("Pixel 2: {}, {}, {}", p2.ch1, p2.ch2, p2.ch3);
    // std::println("Pixel 3: {}, {}, {}", p3.ch1, p3.ch2, p3.ch3);

    

    // std::ifstream ifile(ifilep);
    // std::filesystem::path ofilep = std::filesystem::weakly_canonical(config->config.user_directory) /= "reconstructed.txt";
    // std::println("ofile: {}", ofilep.string());
    // std::ofstream ofile(ofilep);
    // auto tokens = core::internal::tokenize(ifile);
    // core::internal::reconstruct_file(tokens, ofile);

    ini::save_ini(config_path, *config);

    return EXIT_SUCCESS;
}
