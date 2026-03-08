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

#include <axel/grammar.h> // for Grammar
#include <deque>          // for deque
#include <string>         // for basic_string, string
#include <string_view>    // for string_view
#include <utility>        // for move
namespace peg {
class parser;
} // lines 21-21

namespace axel {

struct Instruction
{
    std::string mnemonic;
    std::string addressing_mode;
    std::deque<std::string> operands;
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
    using Instructions = std::deque<Instruction>;

  public:
    Instructions parse(std::string_view assembly);

  private:
    void from_label(peg::parser& parser);
    void from_mnemonic(peg::parser& parser);
    void from_operands(peg::parser& parser);
    void from_operand(peg::parser& parser);

  private:
    Instructions instructions_{};
    grammar::Grammar grammar_;
};

} // namespace axel