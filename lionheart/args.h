/*****************************************************************************
 * Copyright (c) Jahan Addison
 *
 * This software is dual-licensed under the Apache License, Version 2.0 or
 * the GNU General Public License, Version 3.0 or later.
 *
 * You may use this work, in part or in whole, under the terms of either
 * license.
 *
 * See the LICENSE.Apache-v2 and LICENSE.GPL-v3 files in the project root
 * for the full text of these licenses.
 ****************************************************************************/

#include <iostream>    // for cerr
#include <optional>    // for optional
#include <span>        // for span
#include <string_view> // for basic_string_view, string_view

namespace lionheart::args {

struct Config
{
    std::string_view input_file;
    std::optional<std::string_view> output_file;
};

std::optional<Config> parse_args(int argc, const char* const* argv)
{
    std::span<const char* const> args{ argv, static_cast<size_t>(argc) };

    if (argc < 2) {
        std::cerr << "\033[31mError: missing input file.\033[0m\n"
                  << "Usage: " << args[0] << " <input_file> [output_file]\n";
        return std::nullopt;
    }

    Config config{};

    config.input_file = args[1];

    if (argc >= 3)
        config.output_file = args[2];
    if (argc > 3)
        std::cerr << "\033[33mWarning: extra arguments ignored.\033[0m\n";

    return config;
}

} // namespace args