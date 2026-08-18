// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 STupidLord

#include "arg.hpp"

#include <vector>
#include <string_view>
#include <print>

#include "ini.hpp"

#include <project_version.hpp>
#include <cli_version.hpp>
#include <core_version.hpp>

namespace arg {
void handle_help(ini::ini& config,
                 const std::vector<std::string_view>& args) {
    std::println(
        LN_TM_STR " {}.{}.{} \n"
        LN_CR_STR  "\n"
        LN_GPL_STR "\n",
        LN_V_MAJOR, LN_V_MINOR, LN_V_PATCH
    );
    std::println("Usage: (libre-)nudge [options]");
    for (const command& cmd : COMMANDS) {
        if (!cmd.char_flag.empty() && !cmd.str_flag.empty()) {
            std::println("  {}, {} {}",
                         cmd.char_flag, cmd.str_flag, cmd.argument_hint);
        } else if (!cmd.char_flag.empty()) {
            std::println("  {} {}",
                         cmd.char_flag, cmd.argument_hint);
        } else {
            std::println("      {} {}",
                         cmd.str_flag, cmd.argument_hint);
        }
        std::println("        {}", cmd.description);
    }
}
void handle_version(ini::ini& config,
                    const std::vector<std::string_view>& args) {
    if (args.empty()) {
        std::println(
            LN_TM_STR " {}.{}.{}\n"
            LN_CR_STR  "\n"
            LN_GPL_STR,
            LN_V_MAJOR, LN_V_MINOR, LN_V_PATCH
        );
    } else if (args.at(0) == "verbose") {
        std::println(
            LN_TM_STR " {}.{}.{}\n"
            LN_CR_STR  "\n"
            LN_GPL_STR "\n",
            LN_V_MAJOR, LN_V_MINOR, LN_V_PATCH
        );

        std::println("Components:");
        std::println(
            "  " LN_CLI_TM_STR " {}.{}.{}",
            LN_CLI_V_MAJOR, LN_CLI_V_MINOR, LN_CLI_V_PATCH
        );
        std::println(
            "  " LN_CORE_TM_STR " {}.{}.{}",
            LN_CORE_V_MAJOR, LN_CORE_V_MINOR, LN_CORE_V_PATCH
        );
    } // TODO: Might need to setup error handling here
}
void handle_game_dir(ini::ini& config,
                     const std::vector<std::string_view>& args) {
    config.config.game_directory = args.at(0);
}
void handle_user_dir(ini::ini& config,
                     const std::vector<std::string_view>& args) {
    config.config.user_directory = args.at(0);
}
}
