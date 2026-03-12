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

#include <doctest/doctest.h> // for ResultBuilder, REQUIRE, filloss, toSt...

#include <lionheart/grammar.h> // for MC6800_GRAMMAR
#include <lionheart/parser.h>  // for Parser
#include <lionheart/symbol.h>  // for Instruction, Symbol, Mnemonic, Mode
#include <string>              // for basic_string
#include <string_view>         // for basic_string_view

TEST_CASE("Parser: Variable")
{
    using namespace lionheart;
    auto good = R"(
        REDIS   = $FE3A
    )";
    auto good_2 = R"(
        AB = $FEA1
        CD = $FEA2
        EF = $FEA3
    )";

    auto bad = R"(
        DIS     = $FE
        REDIS   = DIS
    )";

    auto parser = Parser{ grammar::MC6800_GRAMMAR };
    auto& symbols = parser.symbol_table();
    REQUIRE_NOTHROW(parser.parse(good));
    REQUIRE_EQ(symbols.at("REDIS").value, "$FE3A");
    REQUIRE_EQ(symbols.at("REDIS").type, symbol::Symbol::Type::Variable);
    REQUIRE_THROWS(parser.parse(bad));
    REQUIRE_NOTHROW(parser.parse(good_2));
    REQUIRE_EQ(symbols.at("AB").value, "$FEA1");
    REQUIRE_EQ(symbols.at("CD").value, "$FEA2");
    REQUIRE_EQ(symbols.at("EF").value, "$FEA3");
}

TEST_CASE("Parser: Label")
{
    using namespace lionheart;
    auto test = R"(
        MYLABEL:
    )";

    auto parser = Parser{ grammar::MC6800_GRAMMAR };
    auto& symbols = parser.symbol_table();
    REQUIRE_NOTHROW(parser.parse(test));
    REQUIRE_EQ(symbols.at("MYLABEL").type, symbol::Symbol::Type::Label);
}

TEST_CASE("Parser: Mnemonic")
{
    using namespace lionheart;
    auto test = R"(
        ORG $F000
        REDIS   = $FE3A
        JSR REDIS
	    LDA A #$01
	    BRA OUT
        TAB
	    TST B
    )";
    auto parser = Parser{ grammar::MC6800_GRAMMAR };
    auto& instructions = parser.instructions();

    REQUIRE_NOTHROW(parser.parse(test));
    REQUIRE_EQ(instructions.size(), 6);

    REQUIRE(instructions.at(0).mnemonic == symbol::Mnemonic::ORG);
    REQUIRE(instructions.at(1).mnemonic == symbol::Mnemonic::JSR);
    REQUIRE(instructions.at(2).mnemonic == symbol::Mnemonic::LDA);
    REQUIRE(instructions.at(3).mnemonic == symbol::Mnemonic::BRA);
    REQUIRE(instructions.at(4).mnemonic == symbol::Mnemonic::TAB);
    REQUIRE(instructions.at(5).mnemonic == symbol::Mnemonic::TST);

    REQUIRE(instructions.at(0).mode == symbol::Mode::Wide);
    REQUIRE(instructions.at(1).mode == symbol::Mode::Jump);
    REQUIRE(instructions.at(2).mode == symbol::Mode::AccSrc8);
    REQUIRE(instructions.at(3).mode == symbol::Mode::Branch);
    REQUIRE(instructions.at(4).mode == symbol::Mode::Inherent);
    REQUIRE(instructions.at(5).mode == symbol::Mode::Unary8);

    REQUIRE(instructions.at(0).operands.size() == 1);
    REQUIRE(instructions.at(0).operands.front() == "$F000");

    REQUIRE(instructions.at(1).operands.size() == 1);
    REQUIRE(instructions.at(1).operands.front() == "REDIS");

    REQUIRE(instructions.at(2).operands.size() == 2);
    REQUIRE(instructions.at(2).operands.front() == "A");
    REQUIRE(instructions.at(2).operands.back() == "#$01");

    REQUIRE(instructions.at(3).operands.size() == 1);
    REQUIRE(instructions.at(3).operands.front() == "OUT");

    REQUIRE(instructions.at(4).operands.size() == 0);

    REQUIRE(instructions.at(5).operands.size() == 1);
    REQUIRE(instructions.at(5).operands.front() == "B");
}