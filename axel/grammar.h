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
#include <fmt/format.h> // for format
#include <iterator>     // for next
#include <numeric>      // for accumulate
#include <string>       // for basic_string, allocator, char_traits, string
#include <string_view>  // for basic_string_view, string_view
#include <utility>      // for move

namespace axel {

namespace grammar {

struct rule
{};

struct data_16bit_immediate : rule
{
    constexpr static std::string_view rule = R"(
        data_16bit_immediate <- '#' [0-9a-fA-F]{4}
    )";
};

struct data_16bit_direct : rule
{
    constexpr static std::string_view rule = R"(
        data_16bit_direct <- [0-9a-fA-F]{4}
    )";
};

struct data_8bit_immediate : rule
{
    constexpr static std::string_view rule = R"(
        data_8bit_immediate <- '#' [0-9a-fA-F]{2}
    )";
};

struct data_8bit_direct : rule
{
    constexpr static std::string_view rule = R"(
        data_8bit_direct <- [0-9a-fA-F]{2}
    )";
};

struct displacement : rule
{
    constexpr static std::string_view rule = R"(
        displacement <- [a-zA-Z][a-zA-Z0-9]*
    )";
};

struct Register : rule
{
    constexpr static std::string_view rule = R"(
        REGISTER <- 'A' | 'B' | 'X' | 'PC' | 'SP' | 'SR'
    )";
};

struct Label : rule
{
    constexpr static std::string_view rule = R"(
        LABEL <- [a-zA-Z][a-zA-Z0-9]*
    )";
};

struct Operands : rule
{
    constexpr static std::string_view rule = R"(
        OPERANDS <- OPERAND (',' OPERAND)*
    )";
};

struct Operand : rule
{
    constexpr static std::string_view rule = R"(
        OPERAND <- data_16bit_immediate /
            REGISTER /
            data_16bit_direct /
            data_8bit_immediate /
            data_8bit_direct /
            Displacement
    )";
};

struct Mnemonic : rule
{
    constexpr static std::string_view rule = R"(
        MNEMONIC = 'ABA' |
        'ADC' |
        'ADD' |
        'AND' |
        'ASL' |
        'ASR' |
        'BCC' |
        'BCS' |
        'BEQ' |
        'BGE' |
        'BGT' |
        'BHI' |
        'BIT' |
        'BLE' |
        'BLS' |
        'BLT' |
        'BMI' |
        'BNE' |
        'BPL' |
        'BRA' |
        'BSR' |
        'BVC' |
        'BVS' |
        'CBA' |
        'CLC' |
        'CLI' |
        'CLR' |
        'CLV' |
        'CMP' |
        'COM' |
        'CPX' |
        'DAA' |
        'DEC' |
        'DES' |
        'DEX' |
        'EOR' |
        'INC' |
        'INS' |
        'INX' |
        'JMP' |
        'JSR' |
        'LDA' |
        'LDS' |
        'LDX' |
        'LSR' |
        'NEG' |
        'NOP' |
        'ORA' |
        'PSH' |
        'PUL' |
        'ROL' |
        'ROR' |
        'RTI' |
        'RTS' |
        'SBA' |
        'SBC' |
        'SEC' |
        'SEI' |
        'SEV' |
        'STA' |
        'STS' |
        'STX' |
        'SUB' |
        'SWI' |
        'TAB' |
        'TAP' |
        'TBA' |
        'TPA' |
        'TST' |
        'TSX' |
        'TXS' |
        'WAI'
    )";
};

// LABEL[space]MNEMO-CODE[space]PARAMETER/S[space]COMMENT

struct Grammar
{
    std::string operator()()
    {
        std::string grammer_rules = std::accumulate(std::next(rules.begin()),
            rules.end(),
            std::string(rules[0]),
            [](std::string a, std::string_view b) {
                return std::move(a) + " / " + std::string(b);
            });
        return fmt::format(R"(
            Instruction <- LABEL? MNEMONIC Operands?

            # Rule Expansion
            {}

            Comment     <- ';' [^\n]*
            Ignore      <- ( [ \t\r\n] / Comment )*

            %whitespace <- Ignore
        )",
            grammer_rules);
    }

  private:
    // clang-format off
    const std::array<std::string_view, 9> rules = {
        Mnemonic::rule,
        Operands::rule,
        Label::rule,
        Operand::rule,
        data_16bit_immediate::rule,
        data_16bit_direct::rule,
        data_8bit_immediate::rule,
        data_8bit_direct::rule,
        displacement::rule
    };
    // clang-format on
};

constexpr auto MC6800_GRAMMAR = Grammar();

} // namespace grammar

} // namespace axel