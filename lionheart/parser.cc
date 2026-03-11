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

#include <lionheart/parser.h>

#include <fmt/base.h>          // for println
#include <functional>          // for function
#include <lionheart/grammar.h> // for Grammar
#include <lionheart/util.h>    // for get_boundary_substr
#include <peglib.h>            // for parser, Action, Definition, SemanticV...
#include <string>              // for basic_string, char_traits, string

namespace lionheart {

Parser::Instructions Parser::parse(std::string_view assembly)
{
    pegparser_ = peg::parser();
    // fmt::println("Grammar: {}", grammar_());
    pegparser_.set_logger(
        [&](std::size_t line, std::size_t col, std::string const& msg
            /*, std::string const& rule*/) {
            /*fmt::println("Rule: {}", rule);*/
            throw Parser_Error(line, col, msg);
        });
    auto test = pegparser_.load_grammar(grammar_());
    if (!test)
        throw Parser_Error(0, 0, "Bad grammar");

    pegparser_.enable_packrat_parsing();
    from_variable();
    from_label();
    pegparser_.parse(assembly);
    return instructions_;
}

void Parser::from_label()
{
    pegparser_["Label"] = [&](peg::SemanticValues const& vs) {
        auto token =
            std::string{ vs.token().substr(0, vs.token().length() - 1) };
        detail::Symbol symbol = { .value = token,
            .type = detail::Symbol::Type::Label,
            .line = vs.line_info().first };

        symbols_.emplace(token, symbol);
    };
}

void Parser::from_variable()
{
    pegparser_["Variable"] = [&](peg::SemanticValues const& vs) {
        std::string token = vs.token().data();
        auto symbol_name = util::get_boundary_substr(token);
        auto symbol_value = util::get_boundary_substr(
            token.substr(token.find_first_of("=") + 1));

        detail::Symbol symbol = { .value = symbol_value,
            .type = detail::Symbol::Type::Variable,
            .line = vs.line_info().first };

        symbols_.emplace(symbol_name, symbol);
    };
}

} // namespace lionheart