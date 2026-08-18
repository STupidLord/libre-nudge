// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 STupidLord

#include "arg.hpp"

#include <cstdlib>
#include <functional>
#include <vector>
#include <string_view>
#include <cstddef>
#include <print>

// This argument parser was originally from one of my C projects,
// however I had decided it was a bit annoying and such completely
// rewrote it. Did I need to? Not really, I just considered I could
// do it differently and so I did.

namespace arg {
namespace {
// Internal logic after the matching here should probably be split
// into another function because it's identical, it's tiring to
// update both everytime.
command_return str_flag(ini::ini& config, size_t pos,
                        const std::vector<std::string_view>& args) {
    command_return cr{};
    std::vector<std::string_view> pass_args{};
    int potential_arguments = args.size() - 1 - pos;
    for (const command& cmd : COMMANDS) {
        if (cmd.str_flag.empty()) continue;
        if (args.at(pos) == cmd.str_flag) {
            if (potential_arguments < cmd.expected_args) {
                cr.argument = pos;
                cr.code = command_return_code::FAILURE_EXPECTED_VALUE;
                return cr;
            }
            switch (cmd.expected_args) {
            case 0: cmd.handler(config, pass_args); break;
            case -1:
                if (potential_arguments > 0 && args.at(pos + 1).at(0) != '-')  {
                    pass_args.push_back(args.at(pos + 1));
                    cr.argument = 1;
                }
                cmd.handler(config, pass_args);
                break;
            default:
                pass_args.push_back(args.at(pos + cmd.expected_args));
                cmd.handler(config, pass_args);
                cr.argument = cmd.expected_args;
                break;
            }
            if (cmd.exit_early) cr.code = command_return_code::EARLY_EXIT;
            else cr.code = command_return_code::SUCCESS;
            return cr;
        }
    }
    cr.code = command_return_code::FAILURE_UNKNOWN;
    return cr;
}
command_return char_flag(ini::ini& config, size_t pos,
                         const std::vector<std::string_view>& args) {
    command_return cr{};
    std::vector<std::string_view> pass_args{};
    int potential_arguments = args.size() - 1 - pos;
    for (const command& cmd : COMMANDS) {
        if (cmd.char_flag.empty()) continue;
        if (args.at(pos).at(1) == cmd.char_flag.at(1)) {
            if (potential_arguments < cmd.expected_args) {
                cr.argument = pos;
                cr.code = command_return_code::FAILURE_EXPECTED_VALUE;
                return cr;
            }
            switch (cmd.expected_args) {
            case 0: cmd.handler(config, std::vector<std::string_view>()); break;
            case -1:
                if (args.at(pos + 1).at(0) != '-')  {
                    pass_args.push_back(args.at(pos + 1));
                    cr.argument = 1;
                }
                cmd.handler(config, pass_args);
                break;
            default:
                pass_args.push_back(args.at(pos + cmd.expected_args));
                cmd.handler(config, pass_args);
                cr.argument = cmd.expected_args;
                break;
            }
            if (cmd.exit_early) cr.code = command_return_code::EARLY_EXIT;
            else cr.code = command_return_code::SUCCESS;
            return cr;
        }
    }
    cr.argument = pos;
    cr.code = command_return_code::FAILURE_UNKNOWN;
    return cr;
}
}

// TODO: See if this could be split up?
// It's not really big, but it's annoyingly deep.

command_return parse_args(ini::ini& config,
                          const std::vector<std::string_view>& args) {
    command_return cr{};
    for (int i = 0; i < args.size(); i++) {
        switch (args.at(i).at(1)) {
        case '-': cr = str_flag(config, i, args); break;
        default:
            switch (args.at(i).at(0)) {
            case '-':
                if (args.at(i).size() > 2) {
                    cr.argument = i;
                    cr.code = command_return_code::FAILURE_CHAR_FLAG_BIG;
                    return cr;
                }
                cr = char_flag(config, i, args); break;
            default:
                cr.argument = i;
                cr.code = command_return_code::FAILURE_EXPECTED_FLAG;
                return cr;
            }
        }
        if (cr.code == command_return_code::EARLY_EXIT
         || cr.code == command_return_code::FAILURE_UNKNOWN
         || cr.code == command_return_code::FAILURE_EXPECTED_VALUE) return cr;
        if (cr.argument > 0) {
            i += cr.argument;
            cr.argument = 0;
        }
    }
    return cr;
}

std::vector<std::string_view> args_from_argv(int argc, char* argv[]) {
    return std::vector<std::string_view>(argv + 1, argv + argc);
}

int handle_error(const command_return& cr,
                 const std::vector<std::string_view>& args) {
    switch (cr.code) {
    case command_return_code::SUCCESS: return 0;
    case command_return_code::FAILURE_UNKNOWN: 
        std::println(stderr, "Invalid argument: {}",
                     args.at(cr.argument));
        return EXIT_FAILURE;
    case command_return_code::FAILURE_EXPECTED_FLAG: 
        std::println(stderr,
                     "Invalid argument, expected flag: {}",
                     args.at(cr.argument));
        return EXIT_FAILURE;
    case command_return_code::FAILURE_CHAR_FLAG_BIG: 
        std::println(stderr,
                     "Invalid argument, expected single character flag: {}",
                     args.at(cr.argument));
        return EXIT_FAILURE;
    case command_return_code::FAILURE_EXPECTED_VALUE: 
        std::println(stderr,
                     "Expected value for argument: {}",
                     args.at(cr.argument));
        return EXIT_FAILURE;
    case command_return_code::EARLY_EXIT: 
        return EXIT_FAILURE;
    }
}
};
