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

#include <cstddef>              // for size_t
#include <fmt/format.h>         // for format
#include <lionheart/assembly.h> // for Instructions, Symbols
#include <lionheart/grammar.h>  // for grammar_t
#include <lionheart/util.h>     // for PRIVATE_UNLESS_TESTED
#include <optional>             // for optional
#include <peglib.h>             // for parser
#include <stdexcept>            // for runtime_error
#include <string>               // for basic_string, string
#include <string_view>          // for basic_string_view, string_view

namespace lionheart {

/**
 * @brief
 *   Parser Error Exception
 */
class Parser_Error : public std::runtime_error
{
  public:
    Parser_Error(std::size_t line, std::size_t col, std::string const& message)
        : std::runtime_error(
              fmt::format(">>> Parser Error:\n    on line {}:{} with: {}",
                  line,
                  col,
                  message))
    {
    }
};

/**
 * @brief
 *   MC6800 Assembly Parser
 */
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

  public:
    assembly::Instructions parse(std::string_view assembly);

  public:
    assembly::Symbols& symbol_table() { return symbols_; }
    assembly::Instructions& instructions() { return instructions_; }

    // clang-format off
  PRIVATE_UNLESS_TESTED:
    void from_variable();
    void from_label();
    // clang-format on
    void from_mnemonic();
    void from_operands();

  private:
    assembly::Instructions instructions_{};
    grammar::grammar_t grammar_;
    assembly::Symbols symbols_{};
    peg::parser pegparser_{};
};

} // namespace lionheart