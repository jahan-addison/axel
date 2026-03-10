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

#include <lionheart/parser.h>

#include <assert.h>            // for assert
#include <lionheart/grammar.h> // for Grammar
#include <peglib.h>            // for parser
#include <string>              // for basic_string, char_traits

namespace lionheart {

Parser::Instructions Parser::parse(std::string_view assembly)
{
    auto parser = peg::parser(grammar_());
    // fmt::println("Grammar: {}", grammar_());
    parser.set_logger(
        [&](std::size_t line, std::size_t col, std::string const& msg
            /*, std::string const& rule*/) {
            /*fmt::println("Rule: {}", rule);*/
            throw Parser_Error(line, col, msg);
        });

    parser.enable_packrat_parsing();
    parser.parse(assembly);
    return instructions_;
}

} // namespace lionheart