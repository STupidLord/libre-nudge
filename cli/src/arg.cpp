// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 STupidLord

#include "arg.hpp"

#include <cstdlib>
#include <tuple>
#include <vector>
#include <string_view>
#include <cstddef>
#include <optional>
#include <print>

#define R_SUCCESS (argv_result)\
                  {argv_error::ERR_SUCCESS,\
                   std::nullopt, std::nullopt, flags, values}
#define R_ERR_ARG (argv_result)\
                  {argv_error::ERR_ARG,\
                   args.at(i), std::nullopt, flags, values}
#define R_ERR_ARG_SIZE (argv_result)\
                       {argv_error::ERR_ARG_SIZE,\
                        args.at(i), std::nullopt, flags, values}
#define R_ERR_ARG_CONFLICT(conflict_arg) (argv_result)\
                          {argv_error::ERR_ARG_CONFLICT,\
                           args.at(i), conflict_arg, flags, values}
#define R_ERR_ARG_EXPECTED_VALUE (argv_result)\
                                 {argv_error::ERR_ARG_EXPECTED_VALUE,\
                                  args.at(i), std::nullopt, flags, values}
#define R_ERR_ARG_EXPECTED_VALUE_GOT(bad_value) (argv_result)\
                                    {argv_error::ERR_ARG_EXPECTED_VALUE_GOT,\
                                     args.at(i), bad_value, flags, values}

namespace arg {
namespace {
// Might need to implement a collect all and collect multiple eventually
/**
* @return Returns a size_t equal to the amount of values collected, returns
*         1 if a value is collected and 0 if no value is collected.
*/
auto collect_value(const std::vector<std::string_view> args,
                    const int current) // Evil wrapping :sob:
                    -> std::tuple<std::size_t,
                                    std::optional<std::string_view>> {
    std::size_t checkout = current + 1;

    if (checkout >= args.size() || args.at(checkout).front() == '-')
        return {0, std::nullopt};

    return {1, args.at(checkout)};
}
};

std::vector<std::string_view> args_from_argv(int argc, char* argv[]) {
    return std::vector<std::string_view>(argv, argv + argc);
}

argv_result parse_args(const std::vector<std::string_view> args) {
    argv_flags flags{};
    argv_values values{};

    for (int i = 1; i < args.size(); i++) {
        if (args.at(i).at(1) == '-') {
            if (args.at(i) == "--version") {
                flags.print_version = true;
                return R_SUCCESS; // Return early on version request
            } else if (args.at(i) == "--help") {
                flags.print_help = true;
                return R_SUCCESS; // Like version, return early on help request
            } else if (args.at(i) == "--game") {
                if (i+1 >= args.size()) return R_ERR_ARG_EXPECTED_VALUE;

                auto [read, value] = collect_value(args, i);
                if (!read) return R_ERR_ARG_EXPECTED_VALUE_GOT(args.at(i+1));

                values.game_directory = value.value();
                i += read;
            } else if (args.at(i) == "--user") {
                if (i+1 >= args.size()) return R_ERR_ARG_EXPECTED_VALUE;

                auto [read, value] = collect_value(args, i);
                if (!read) return R_ERR_ARG_EXPECTED_VALUE_GOT(args.at(i+1));

                values.user_directory = value.value();
                i += read;
            } else {
                return R_ERR_ARG;
            }
        } else if (args.at(i).front() == '-') {
            if (args.at(i).size() > 2) return R_ERR_ARG_SIZE;
            switch (args.at(i).at(1)) {
            case 'v':
                flags.print_version = true;
                return R_SUCCESS;
            case 'h':
                flags.print_help = true;
                return R_SUCCESS;
            default:
                return R_ERR_ARG;
            }
        } else return R_ERR_ARG;
    }
    return R_SUCCESS;
}

int handle_error(const argv_result& result) {
    switch (result.error_code) {
    case arg::argv_error::ERR_SUCCESS:
        break;
    case arg::argv_error::ERR_ARG:
        std::println(stderr, "Invalid argument: {}",
                     result.bad_arg.value_or(""));
        return EXIT_FAILURE;
    case arg::argv_error::ERR_ARG_SIZE:
        std::println(stderr, "Invalid argument, expected single character flag: {}",
                     result.bad_arg.value_or(""));
        return EXIT_FAILURE;
    case arg::argv_error::ERR_ARG_CONFLICT:
        std::println(stderr, "Conflicting arguments: {} with {}",
                     result.bad_arg.value_or(""),
                     result.bad_arg_2.value_or(""));
        return EXIT_FAILURE;
    case arg::argv_error::ERR_ARG_EXPECTED_VALUE:
        std::println(stderr, "Expected value for argument: {}",
                     result.bad_arg.value_or(""));
        return EXIT_FAILURE;
    case arg::argv_error::ERR_ARG_EXPECTED_VALUE_GOT:
        std::println(stderr, "Expected value for argument: {} got {}",
                     result.bad_arg.value_or(""),
                     result.bad_arg_2.value_or(""));
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
};
