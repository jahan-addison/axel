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

#include <array>        // for array
#include <axel/util.h>  // for AXEL_PRIVATE_UNLESS_TESTED
#include <fmt/format.h> // for format
#include <iterator>     // for next
#include <numeric>      // for accumulate
#include <string>       // for basic_string, allocator, char_traits, string
#include <string_view>  // for basic_string_view, string_view
#include <utility>      // for move

namespace axel {

namespace grammar {

/***************************************************************************
 * See https://github.com/yhirose/cpp-peglib for details on grammar syntax
 ***************************************************************************/

struct rule
{
    using rule_t = std::string_view;
};

struct hex_immediate_8bit : rule
{
    constexpr static rule_t rule = R"(
        hex_immediate_8bit <- '$' [0-9a-fA-F]{2}
    )";
};

struct octal_immediate_8bit : rule
{
    constexpr static rule_t rule = R"(
        octal_immediate_8bit <- '@' ([0-1][0-7]{2} / [0-7]{1,2})
    )";
};

struct hex_immediate_16bit : rule
{
    constexpr static rule_t rule = R"(
        hex_immediate_16bit <- '$' [0-9a-fA-F]{4}
    )";
};

struct octal_immediate_16bit : rule
{
    constexpr static rule_t rule = R"(
        octal_immediate_16bit <- '@' ([0-1][0-7]{5} / [0-7]{1,5})
    )";
};

struct data_16bit_immediate : rule
{
    constexpr static rule_t rule = R"(
        data_16bit_immediate <- '#' (hex_immediate_16bit / octal_immediate_16bit)
    )";
};

struct data_16bit_direct : rule
{
    constexpr static rule_t rule = R"(
        data_16bit_direct <- (hex_immediate_16bit / octal_immediate_16bit)
    )";
};

struct data_8bit_immediate : rule
{
    constexpr static rule_t rule = R"(
        data_8bit_immediate <- '#' (hex_immediate_8bit / octal_immediate_8bit)
    )";
};

struct data_8bit_direct : rule
{
    constexpr static rule_t rule = R"(
        data_8bit_direct <- (hex_immediate_8bit / octal_immediate_8bit)
    )";
};

struct displacement : rule
{
    constexpr static rule_t rule = R"(
        displacement <- [a-zA-Z][a-zA-Z0-9]*
    )";
};

struct Register : rule
{
    constexpr static rule_t rule = R"(
        REGISTER <- 'A' / 'B' / 'X' / 'PC' / 'SP' / 'SR'
    )";
};

struct Label : rule
{
    constexpr static rule_t rule = R"(
        LABEL <- [a-zA-Z][a-zA-Z0-9]* ':'
    )";
};

struct Variable : rule
{
    constexpr static rule_t rule = R"(
        VARIABLE <- [a-zA-Z][a-zA-Z0-9]* '=' data_16bit_direct
    )";
};

struct Operands : rule
{
    constexpr static rule_t rule = R"(
        OPERANDS <- OPERAND (',' OPERAND)*
    )";
};

struct Operand : rule
{
    constexpr static rule_t rule = R"(
        OPERAND <- data_16bit_immediate /
            REGISTER /
            data_16bit_direct /
            data_8bit_immediate /
            data_8bit_direct /
            displacement
    )";
};

struct Mnemonic : rule
{
    constexpr static rule_t rule = R"(
        MNEMONIC <- 'ABA' /
        'ADC' /
        'ADD' /
        'AND' /
        'ASL' /
        'ASR' /
        'BCC' /
        'BCS' /
        'BEQ' /
        'BGE' /
        'BGT' /
        'BHI' /
        'BIT' /
        'BLE' /
        'BLS' /
        'BLT' /
        'BMI' /
        'BNE' /
        'BPL' /
        'BRA' /
        'BSR' /
        'BVC' /
        'BVS' /
        'CBA' /
        'CLC' /
        'CLI' /
        'CLR' /
        'CLV' /
        'CMP' /
        'COM' /
        'CPX' /
        'DAA' /
        'DEC' /
        'DES' /
        'DEX' /
        'EOR' /
        'INC' /
        'INS' /
        'INX' /
        'JMP' /
        'JSR' /
        'LDA' /
        'LDS' /
        'LDX' /
        'LSR' /
        'NEG' /
        'NOP' /
        'ORA' /
        'PSH' /
        'PUL' /
        'ROL' /
        'ROR' /
        'RTI' /
        'RTS' /
        'SBA' /
        'SBC' /
        'SEC' /
        'SEI' /
        'SEV' /
        'STA' /
        'STS' /
        'STX' /
        'SUB' /
        'SWI' /
        'TAB' /
        'TAP' /
        'TBA' /
        'TPA' /
        'TST' /
        'TSX' /
        'TXS' /
        'WAI'
    )";
};

struct Grammar
{
    std::string operator()(bool test = false) const
    {
        return fmt::format(R"(
        Instruction <- LABEL? MNEMONIC OPERANDS?

        # Rule Expansion
        {}

        Comment     <- ';' [^\n]*
        Ignore      <- ( [ \t\r\n] / Comment )*

        %whitespace <- Ignore
        )",
            rule_expansion(test));
    }
    // clang-format off
  AXEL_PRIVATE_UNLESS_TESTED:
    constexpr std::string rule_expansion(bool test = false) const
    {
        if (test)
            return std::accumulate(std::next(rules.begin()),
                rules.end(),
                std::string{ get_lexeme(rules[0]) },
                [&, this](std::string a, std::string_view b) {
                    if (b.empty())
                        return a;
                    return std::move(a) + " " + std::string{ get_lexeme(b) };
                });
        else
            return std::accumulate(std::next(rules.begin()),
                rules.end(),
                std::string{ rules[0] },
                [](std::string a, std::string_view b) {
                    if (b.empty())
                        return a;
                    return std::move(a) + std::string{ b };
                });
    }
    // clang-format on
    constexpr std::string_view get_lexeme(std::string_view s) const
    {
        size_t start = s.find_first_not_of(" \t\r\n\v\f");
        if (start == std::string_view::npos)
            return {};
        size_t end = s.find_first_of(" \t\r\n\v\f", start);
        if (end == std::string_view::npos) {
            return s.substr(start);
        }
        return s.substr(start, end - start);
    }

    const std::array<std::string_view, 15> rules = { Mnemonic::rule,
        Variable::rule,
        Operands::rule,
        Label::rule,
        Operand::rule,
        Register::rule,
        data_16bit_immediate::rule,
        data_16bit_direct::rule,
        data_8bit_immediate::rule,
        data_8bit_direct::rule,
        displacement::rule,
        hex_immediate_8bit::rule,
        octal_immediate_8bit::rule,
        hex_immediate_16bit::rule,
        octal_immediate_16bit::rule };
};

constexpr auto MC6800_GRAMMAR = Grammar();

} // namespace grammar

} // namespace axel