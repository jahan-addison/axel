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

#include <deque>               // for deque
#include <lionheart/grammar.h> // for Grammar
#include <lionheart/map.h>     // for Ordered_Map
#include <stdexcept>           // for runtime_error
#include <string>              // for basic_string, string
#include <string_view>         // for string_view
#include <utility>             // for move
namespace peg {
class parser;
}

namespace lionheart {

namespace detail {

struct Instruction
{
    std::string mnemonic;
    std::string addressing_mode;
    std::deque<std::string> operands;
};

struct Symbol
{
    enum class Type
    {
        Variable,
        Label
    };
    std::string value;
    Type type;
    int8_t line;
};

} // namespace detail

class Parser_Error : public std::runtime_error
{
  public:
    Parser_Error(std::size_t line, std::size_t col, std::string const& message)
        : std::runtime_error(fmt::format("Syntax Error - Line {}, col {}\n  {}",
              line,
              col,
              message))
    {
    }
};

class Parser
{
  public:
    Parser() = delete;
    Parser(Parser const&) = delete;
    Parser& operator=(Parser const&) = delete;
    explicit Parser(grammar::Grammar grammar)
        : grammar_(std::move(grammar))
    {
    }

  public:
    using Instructions = std::deque<detail::Instruction>;
    using Symbols = Ordered_Map<std::string, detail::Symbol>;

  public:
    Instructions parse(std::string_view assembly);
    Symbols& symbol_table() const;

  private:
    void from_label(peg::parser& parser);
    void from_variable(peg::parser& parser);
    void from_mnemonic(peg::parser& parser);
    void from_operands(peg::parser& parser);
    void from_operand(peg::parser& parser);

  private:
    Instructions instructions_{};
    grammar::Grammar grammar_;
    Symbols symbols_{};
};

} // namespace lionheart