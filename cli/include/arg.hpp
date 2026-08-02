// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 STupidLord

#pragma once

#include <vector>
#include <optional>
#include <string_view>
#include <filesystem>

namespace arg {
enum class argv_error {
    ERR_SUCCESS                = 0,
    ERR_ARG                    = 1,
    ERR_ARG_SIZE               = 2,
    ERR_ARG_CONFLICT           = 3,
    ERR_ARG_EXPECTED_VALUE     = 4,
    ERR_ARG_EXPECTED_VALUE_GOT = 5
};

struct argv_flags {
    bool print_version;
    bool print_help;
};

struct argv_values {
    std::optional<std::filesystem::path> game_directory;
    std::optional<std::filesystem::path> user_directory;
};

struct argv_result {
    argv_error                      error_code;
    std::optional<std::string_view> bad_arg;
    std::optional<std::string_view> bad_arg_2;
    argv_flags                      flags;
    argv_values                     values;
};

std::vector<std::string_view> args_from_argv(int argc, char* argv[]);
argv_result parse_args(const std::vector<std::string_view> args);
int handle_error(const argv_result& result);
};
