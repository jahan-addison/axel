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

#include <fmt/base.h>         // for println
#include <functional>         // for function
#include <lionheart/symbol.h> // for Symbol
#include <lionheart/util.h>   // for get_boundary_substr
#include <peglib.h>           // for parser, SemanticValues, Action, Defin...
#include <string>             // for basic_string, string
#include <utility>            // for pair

#define MNEMONIC_OPERAND_ACTION_TYPE(x)                                 \
    do {                                                                \
        pegparser_[x] = [](peg::SemanticValues const& vs) -> std::any { \
            return std::string(vs.token());                             \
        };                                                              \
    } while (0)

namespace lionheart {

Parser::Instructions Parser::parse(std::string_view assembly)
{
    pegparser_ = peg::parser();
    pegparser_.set_logger(
        [&](std::size_t line, std::size_t col, std::string const& msg
            /*, std::string const& rule*/) {
            throw Parser_Error(line, col, msg);
        });
    auto test = pegparser_.load_grammar(grammar_);
    if (!test)
        throw Parser_Error(0, 0, "Bad grammar");

    pegparser_.enable_packrat_parsing();
    from_variable();
    from_label();
    from_operands();
    from_mnemonic();
    pegparser_.parse(assembly);
    return instructions_;
}

void Parser::from_label()
{
    pegparser_["Label"] = [&](peg::SemanticValues const& vs) {
        auto token =
            std::string{ vs.token().substr(0, vs.token().length() - 1) };
        symbol::Symbol symbol = { .value = token,
            .type = symbol::Symbol::Type::Label,
            .line = vs.line_info().first };

        symbols_.emplace(token, symbol);
    };
}

void Parser::from_variable()
{
    pegparser_["Variable"] = [&](peg::SemanticValues const& vs) {
        std::string token = std::string{ vs.token() };
        auto symbol_name = util::get_boundary_substr(token);
        auto symbol_value = util::get_boundary_substr(
            token.substr(token.find_first_of("=") + 1));

        symbol::Symbol symbol = { .value = symbol_value,
            .type = symbol::Symbol::Type::Variable,
            .line = vs.line_info().first };

        symbols_.emplace(symbol_name, symbol);
    };
}

void Parser::from_operands()
{
    MNEMONIC_OPERAND_ACTION_TYPE("Accumulator");
    MNEMONIC_OPERAND_ACTION_TYPE("data_8bit_direct");
    MNEMONIC_OPERAND_ACTION_TYPE("data_16bit_direct");
    MNEMONIC_OPERAND_ACTION_TYPE("data_8bit_immediate");
    MNEMONIC_OPERAND_ACTION_TYPE("data_16bit_immediate");
    MNEMONIC_OPERAND_ACTION_TYPE("indexed_address");
    MNEMONIC_OPERAND_ACTION_TYPE("identifier");
}

void Parser::from_mnemonic()
{

    pegparser_["Mnemonic"] = [&](peg::SemanticValues const& vs) {
        using enum symbol::Mode;
        auto addressing_mode = symbol::addressing_mode[vs.choice()];
        symbol::Instruction instruction{
            .mnemonic = symbol::mnemonic_string_index(vs.token()),
            .mode = addressing_mode,
            .operands = symbol::Operands{}
        };
        if (vs.size() >= 1 and vs[0].has_value())
            instruction.operands.emplace_back(
                std::any_cast<std::string>(vs[0]));
        if (vs.size() >= 2 and vs[1].has_value())
            instruction.operands.emplace_back(
                std::any_cast<std::string>(vs[1]));
        if (vs.size() == 3 and vs[2].has_value())
            instruction.operands.emplace_back(
                std::any_cast<std::string>(vs[1]));

        instructions_.emplace_back(instruction);
    };
}

} // namespace lionheart