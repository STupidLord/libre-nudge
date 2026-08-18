// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 STupidLord

#pragma once

#include <vector>
#include <string_view>
#include <functional>

#include "ini.hpp"

namespace arg {
enum class command_return_code {
    SUCCESS,
    EARLY_EXIT,
    FAILURE_UNKNOWN,
    FAILURE_EXPECTED_FLAG,
    FAILURE_CHAR_FLAG_BIG,
    FAILURE_EXPECTED_VALUE,
    // FAILURE_EXPECTED_VALUE_FLAG, // TODO: Expected value, got flag
};

struct command_return {
    command_return_code code;
    size_t              argument = 0;
};

struct command {
    std::string_view char_flag;
    std::string_view str_flag;
    std::string_view argument_hint;
    std::string_view description;
    int expected_args;
    std::function<void(ini::ini& config,
                       const std::vector<std::string_view>& args)> handler;
    bool exit_early = false;
};

void handle_help(ini::ini& config,
                 const std::vector<std::string_view>& args);
void handle_version(ini::ini& config,
                    const std::vector<std::string_view>& args);
void handle_game_dir(ini::ini& config,
                     const std::vector<std::string_view>& args);
void handle_user_dir(ini::ini& config,
                     const std::vector<std::string_view>& args);

const command COMMANDS[] = {
    {
        "-h", "--help", "",
        "Prints a list of all commands",
        0, handle_help, true
    },
    {
        "",   "--version", "[verbose]",
        "Prints version information",
        -1, handle_version, true
    },
    {
        "",   "--game-dir", "<path>",
        "Installation directory of Hearts of Iron IV",
        1, handle_game_dir
    },
    {
        "",   "--user-dir", "<path>",
        "User directory of Hearts of Iron IV",
        1, handle_user_dir
    },
};

std::vector<std::string_view> args_from_argv(int argc, char* argv[]);
command_return parse_args(ini::ini& config,
                          const std::vector<std::string_view>& args);
int handle_error(const command_return& result,
                 const std::vector<std::string_view>& args);
};
