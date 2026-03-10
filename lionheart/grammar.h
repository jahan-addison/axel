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

#include <array>            // for array
#include <fmt/format.h>     // for format
#include <iterator>         // for next
#include <lionheart/util.h> // for AXEL_PRIVATE_UNLESS_TESTED
#include <numeric>          // for accumulate
#include <string>           // for basic_string, allocator, char_traits, string
#include <string_view>      // for basic_string_view, string_view
#include <utility>          // for move

namespace lionheart {

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

struct alpha_numeric_identifier : rule
{
    constexpr static rule_t rule = R"(
        identifier <- < [a-zA-Z][a-zA-Z0-9]* >
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

struct Label : rule
{
    constexpr static rule_t rule = R"(
        Label <- identifier ':'
    )";
};

struct Variable : rule
{
    constexpr static rule_t rule = R"(
        Variable <- identifier '=' data_16bit_direct
    )";
};

struct Accumulator : rule
{
    constexpr static rule_t rule = R"(
        Accumulator <- 'A' / 'B'
    )";
};

struct Indexed_address : rule
{
    constexpr static rule_t rule = R"(
        indexed_address <- data_8bit_direct ',' 'X'
    )";
};

struct Mnemonic : rule
{
    constexpr static rule_t rule = R"(
        Mnemonic <-
            ABA / ADC / ADD / AND / ASL / ASR /
            BCC / BCS / BEQ / BGE / BGT / BHI / BIT / BLE / BLS / BLT / BMI / BNE / BPL /
            BRA / BSR / BVC / BVS /
            CBA / CLC / CLI / CLR / CLV / CMP / COM / CPX /
            DAA / DEC / DES / DEX /
            EOR / INC / INS / INX /
            JMP / JSR /
            LDA / LDS / LDX / LSR /
            NEG / NOP /
            ORA / PSH / PUL /
            ROL / ROR / RTI / RTS /
            SBA / SBC / SEC / SEI / SEV / STA / STS / STX / SUB / SWI /
            TAB / TAP / TBA / TPA / TST / TSX / TXS / WAI

        ABA <- 'ABA'
        ADC <- 'ADC' Accumulator (data_8bit_immediate / indexed_address / data_8bit_direct / data_16bit_direct)
        ADD <- 'ADD' Accumulator (data_8bit_immediate / indexed_address / data_8bit_direct / data_16bit_direct)
        AND <- 'AND' Accumulator (data_8bit_immediate / indexed_address / data_8bit_direct / data_16bit_direct)
        ASL <- 'ASL' (Accumulator / indexed_address / data_16bit_direct)
        ASR <- 'ASR' (Accumulator / indexed_address / data_16bit_direct)
        BCC <- 'BCC' (data_8bit_direct / identifier)
        BCS <- 'BCS' (data_8bit_direct / identifier)
        BEQ <- 'BEQ' (data_8bit_direct / identifier)
        BGE <- 'BGE' (data_8bit_direct / identifier)
        BGT <- 'BGT' (data_8bit_direct / identifier)
        BHI <- 'BHI' (data_8bit_direct / identifier)
        BIT <- 'BIT' Accumulator (data_8bit_immediate / indexed_address / data_8bit_direct / data_16bit_direct)
        BLE <- 'BLE' (data_8bit_direct / identifier)
        BLS <- 'BLS' (data_8bit_direct / identifier)
        BLT <- 'BLT' (data_8bit_direct / identifier)
        BMI <- 'BMI' (data_8bit_direct / identifier)
        BNE <- 'BNE' (data_8bit_direct / identifier)
        BPL <- 'BPL' (data_8bit_direct / identifier)
        BRA <- 'BRA' (data_8bit_direct / identifier)
        BSR <- 'BSR' (data_8bit_direct / identifier)
        BVC <- 'BVC' (data_8bit_direct / identifier)
        BVS <- 'BVS' (data_8bit_direct / identifier)
        CBA <- 'CBA'
        CLC <- 'CLC'
        CLI <- 'CLI'
        CLR <- 'CLR' (Accumulator / indexed_address / data_16bit_direct)
        CLV <- 'CLV'
        CMP <- 'CMP' Accumulator (data_8bit_immediate / indexed_address / data_8bit_direct / data_16bit_direct)
        COM <- 'COM' (Accumulator / indexed_address / data_16bit_direct)
        CPX <- 'CPX' (data_16bit_immediate / indexed_address / data_8bit_direct / data_16bit_direct)
        DAA <- 'DAA'
        DEC <- 'DEC' (Accumulator / indexed_address / data_16bit_direct)
        DES <- 'DES'
        DEX <- 'DEX'
        EOR <- 'EOR' Accumulator (data_8bit_immediate / indexed_address / data_8bit_direct / data_16bit_direct)
        INC <- 'INC' (Accumulator / indexed_address / data_16bit_direct)
        INS <- 'INS'
        INX <- 'INX'
        JMP <- 'JMP' (indexed_address / data_16bit_direct)
        JSR <- 'JSR' (indexed_address / data_16bit_direct)
        LDA <- 'LDA' Accumulator (data_8bit_immediate / indexed_address / data_8bit_direct / data_16bit_direct)
        LDS <- 'LDS' (data_16bit_immediate / indexed_address / data_8bit_direct / data_16bit_direct)
        LDX <- 'LDX' (data_16bit_immediate / indexed_address / data_8bit_direct / data_16bit_direct)
        LSR <- 'LSR' (Accumulator / indexed_address / data_16bit_direct)
        NEG <- 'NEG' (Accumulator / indexed_address / data_16bit_direct)
        NOP <- 'NOP'
        ORA <- 'ORA' Accumulator (data_8bit_immediate / indexed_address / data_8bit_direct / data_16bit_direct)
        PSH <- 'PSH' Accumulator
        PUL <- 'PUL' Accumulator
        ROL <- 'ROL' (Accumulator / indexed_address / data_16bit_direct)
        ROR <- 'ROR' (Accumulator / indexed_address / data_16bit_direct)
        RTI <- 'RTI'
        RTS <- 'RTS'
        SBA <- 'SBA'
        SBC <- 'SBC' Accumulator (data_8bit_immediate / indexed_address / data_8bit_direct / data_16bit_direct)
        SEC <- 'SEC'
        SEI <- 'SEI'
        SEV <- 'SEV'
        STA <- 'STA' Accumulator (indexed_address / data_8bit_direct / data_16bit_direct)
        STS <- 'STS' (indexed_address / data_8bit_direct / data_16bit_direct)
        STX <- 'STX' (indexed_address / data_8bit_direct / data_16bit_direct)
        SUB <- 'SUB' Accumulator (data_8bit_immediate / indexed_address / data_8bit_direct / data_16bit_direct)
        SWI <- 'SWI'
        TAB <- 'TAB'
        TAP <- 'TAP'
        TBA <- 'TBA'
        TPA <- 'TPA'
        TST <- 'TST' (Accumulator / indexed_address / data_16bit_direct)
        TSX <- 'TSX'
        TXS <- 'TXS'
        WAI <- 'WAI'
    )";
};

struct Grammar
{
    std::string operator()(bool test = false) const
    {
        return fmt::format(R"(
        Start <- Instruction*
        Instruction <- Variable / Label / Mnemonic

        # Rule Expansion

        {}

        %word <- identifier
        %whitespace <- ( [\s] / comment )*
        comment     <- ';' [^\n]*
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

    const std::array<std::string_view, 14> rules = { Mnemonic::rule,
        Variable::rule,
        Label::rule,
        Accumulator::rule,
        Indexed_address::rule,
        data_16bit_immediate::rule,
        data_16bit_direct::rule,
        data_8bit_immediate::rule,
        data_8bit_direct::rule,
        hex_immediate_8bit::rule,
        octal_immediate_8bit::rule,
        alpha_numeric_identifier::rule,
        hex_immediate_16bit::rule,
        octal_immediate_16bit::rule };
};

constexpr auto MC6800_GRAMMAR = Grammar();

} // namespace grammar

} // namespace lionheart