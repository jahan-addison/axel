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

#pragma once

#include <array>               // for array
#include <cstddef>             // for size_t
#include <deque>               // for deque
#include <fmt/format.h>        // for format
#include <lionheart/grammar.h> // for grammar_t
#include <lionheart/map.h>     // for Ordered_Map
#include <lionheart/symbol.h>  // for Mode, Instruction, Symbol
#include <lionheart/util.h>    // for PRIVATE_UNLESS_TESTED
#include <peglib.h>            // for parser
#include <stdexcept>           // for runtime_error
#include <string>              // for basic_string, string
#include <string_view>         // for basic_string_view, string_view

namespace lionheart {

namespace symbol {

} // namespace symbol

class Parser_Error : public std::runtime_error
{
  public:
    Parser_Error(std::size_t line, std::size_t col, std::string const& message)
        : std::runtime_error(
              fmt::format("\n>>> Line {}:{} - {}", line, col, message))
    {
    }
};

class Parser
{
  public:
    Parser() = delete;
    Parser(Parser const&) = delete;
    Parser& operator=(Parser const&) = delete;
    explicit Parser(grammar::grammar_t const& grammar)
        : grammar_(grammar)
    {
    }
    using Instructions = std::deque<symbol::Instruction>;
    using Symbols = Ordered_Map<std::string, symbol::Symbol>;

  public:
    Instructions parse(std::string_view assembly);

  public:
    inline Symbols& symbol_table() { return symbols_; }
    inline Instructions& instructions() { return instructions_; }

    // clang-format off
  PRIVATE_UNLESS_TESTED:
    void from_variable();
    void from_label();
    // clang-format on
    void from_mnemonic();
    void from_operands();

  private:
    Instructions instructions_{};
    grammar::grammar_t grammar_;
    Symbols symbols_{};
    peg::parser pegparser_{};
};

} // namespace lionheart