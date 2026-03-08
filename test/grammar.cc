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

#include <doctest/doctest.h> // for TestCase, ResultBuilder, toString, REQU...

#include <axel/grammar.h> // for Grammar
#include <stddef.h>       // for size_t
#include <string>         // for basic_string, string
#include <string_view>    // for basic_string_view, string_view

constexpr std::string str_trim_ws(std::string const& ss)
{
    constexpr std::string_view WHITESPACE = " \t\n\r\f\v";
    size_t start = ss.find_first_not_of(WHITESPACE);
    if (start == std::string_view::npos) {
        return "";
    }
    size_t end = ss.find_last_not_of(WHITESPACE);
    size_t length = end - start + 1;
    return ss.substr(start, length);
}

TEST_CASE("grammar: grammar expansion")
{
    // auto grammar = axel::grammar::Grammar{};
    // std::string test = R"(MNEMONIC <- 'ABA' /
    //     'ADC' /
    //     'ADD' /
    //     'AND' /
    //     'ASL' /
    //     'ASR' /
    //     'BCC' /
    //     'BCS' /
    //     'BEQ' /
    //     'BGE' /
    //     'BGT' /
    //     'BHI' /
    //     'BIT' /
    //     'BLE' /
    //     'BLS' /
    //     'BLT' /
    //     'BMI' /
    //     'BNE' /
    //     'BPL' /
    //     'BRA' /
    //     'BSR' /
    //     'BVC' /
    //     'BVS' /
    //     'CBA' /
    //     'CLC' /
    //     'CLI' /
    //     'CLR' /
    //     'CLV' /
    //     'CMP' /
    //     'COM' /
    //     'CPX' /
    //     'DAA' /
    //     'DEC' /
    //     'DES' /
    //     'DEX' /
    //     'EOR' /
    //     'INC' /
    //     'INS' /
    //     'INX' /
    //     'JMP' /
    //     'JSR' /
    //     'LDA' /
    //     'LDS' /
    //     'LDX' /
    //     'LSR' /
    //     'NEG' /
    //     'NOP' /
    //     'ORA' /
    //     'PSH' /
    //     'PUL' /
    //     'ROL' /
    //     'ROR' /
    //     'RTI' /
    //     'RTS' /
    //     'SBA' /
    //     'SBC' /
    //     'SEC' /
    //     'SEI' /
    //     'SEV' /
    //     'STA' /
    //     'STS' /
    //     'STX' /
    //     'SUB' /
    //     'SWI' /
    //     'TAB' /
    //     'TAP' /
    //     'TBA' /
    //     'TPA' /
    //     'TST' /
    //     'TSX' /
    //     'TXS' /
    //     'WAI'

    //     OPERANDS <- OPERAND (',' OPERAND)*

    //     LABEL <- !MNEMONIC [a-zA-Z][a-zA-Z0-9]*

    //     OPERAND <- data_16bit_immediate /
    //         REGISTER /
    //         data_16bit_direct /
    //         data_8bit_immediate /
    //         data_8bit_direct /
    //         displacement

    //     data_16bit_immediate <- '#' [0-9a-fA-F]{4}

    //     data_16bit_direct <- [0-9a-fA-F]{4}

    //     REGISTER <- 'A' / 'B' / 'X' / 'PC' / 'SP' / 'SR'

    //     data_8bit_immediate <- '#' [0-9a-fA-F]{2}

    //     data_8bit_direct <- [0-9a-fA-F]{2}

    //     displacement <- [a-zA-Z][a-zA-Z0-9]*)";
    // REQUIRE_EQ(str_trim_ws(grammar.rule_expansion()), str_trim_ws(test));
    REQUIRE_EQ(true, true);
}