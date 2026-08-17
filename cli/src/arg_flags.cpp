// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 STupidLord

#include "arg.hpp"

#include <vector>
#include <string_view>
#include <print>

#include "ini.hpp"

#include "version.hpp"
#include <libre-nudge/version.hpp>

namespace arg {
void handle_help(ini::ini& config,
                 const std::vector<std::string_view>& args) {
    std::println(
        LN_TMV_STR "\n"
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
