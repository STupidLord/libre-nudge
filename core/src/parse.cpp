// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 STupidLord

#include "parse.hpp"

#include <string>
#include <vector>
#include <fstream>

namespace core {
namespace internal {
namespace {
bool is_comment(char ch) {
    switch (ch) {
    case '#':
        return true;
    case ';':
        return true;
    case '!':
        return true;
    }
    return false;
}
}

std::vector<token> tokenize(std::ifstream& file) {
    std::vector<token> tokens;
    char ch;

    // I hate this
    // Six indents, file I/O is the spawn of Satan
    while (file >> ch) {
        if (is_comment(ch)) {
            std::string comment;
            std::getline(file, comment);
            continue;
        }
        else if (ch == '=') {
            tokens.push_back({token_type::equals, "="});
        } else if (ch == '{') {
            tokens.push_back({token_type::left_bracket, "{"});
        } else if (ch == '}') {
            tokens.push_back({token_type::right_bracket, "}"});
        } else {
            std::string id;
            id += ch;
            if (ch == '"') {
                while (file.get(ch)) {
                    id += ch;
                    if (ch == '"') {
                        break;
                    }
                }
            } else {
                while (file.get(ch)) {
                    if (std::isspace(ch)
                        || ch == '=' || ch == '{' || ch == '}'
                        || is_comment(ch)) {
                        file.unget();
                        break;
                    }
                    id += ch;
                }
            }
            tokens.push_back({token_type::identifier, id});
        }
    }

    return tokens;
}

// Verbosely explained for the sake of helping someone learn
void reconstruct_file(std::vector<token>& tokens, std::ofstream& file) {
    // The previous token, and the token before that.
    // These are needed to properly keep track of values.
    // e.g. in "disabled_in_dmz = yes" `disabled_in_dmz` 
    // would be the last_last_token, `=` would be the last_token,
    // and `yes` would be the current token.
    token_type last_token;
    token_type last_last_token;
    // How many brackets we are deep, i.e. how many indents we need.
    int indent_count{0};

    // We iterate through every token in our list ("vector") of tokens
    // A token has a token_type (enum class) and a string
    // The token_types ares:
    // identifier
    // equals
    // left_bracket
    // right_bracket
    for (const token& token : tokens) {
        // Since enums are just numbers, we can use them in a switch
        // this is faster than if we had stings and had to do multiple
        // string comparisons for every token
        switch (token.token) {
        // identifier tokens are left hand values (keys before `=` e.g.
        // "base_cost") or right hand values (values after `=` e.g. "6000")
        case token_type::identifier:
            // if the last_token was an equals we don't indent
            // and add a newline character
            if (last_token == token_type::equals)
                // the `<<` operator puts characters into a stream,
                // in this case it is an ofstream (output, file, stream)
                // the inverse, `>>`, does as you'd expect and pulls
                // characters from a stream
                file << token.value << "\n";
            // if the last_token wasn't an equals, we indent
            // for indent_count and we don't add a newline charcter
            else {
                for (int i = 0; i < indent_count; i++) file << "    ";
                file << token.value;
            }
            break; // Almost always break out of cases ealier
        // equals, left_bracket, and right_bracket are self-explanatory
        // equals is `=`
        // left_bracket is `{`
        // right_bracket is `}`
        case token_type::equals:
            file << token.value;
            break;
        case token_type::left_bracket:
            // For every left_bracket, we indent once
            indent_count++;
            file << token.value << "\n";
            break;
        case token_type::right_bracket:
            // For every right_bracket, we unindent once
            indent_count--;
            // In the following two ifs, we prepend a newline character
            // in the case of nested blocks that don't have lhv-rhv
            // (e.g. `enable_for_controllers` which is a list of tags)
            //
            // Looking back on this, I'm not 100% sure if the identifiers
            // in that case would properly be separated themselves
            if (last_token == token_type::identifier
                && last_last_token == token_type::left_bracket)
                file << "\n";
            else if (last_token == token_type::identifier
                     && last_last_token == token_type::identifier)
                file << "\n";
            for (int i = 0; i < indent_count; i++) file << "    ";
            file << token.value << "\n";
            break;
        }
        // Shift the last tokens back once we are done with this current token
        last_last_token = last_token;
        last_token = token.token;
    }
}
}
}
