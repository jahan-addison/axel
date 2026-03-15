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

#include <lionheart/assembly.h> // for Instruction, Symbol, Mnemonic, Mode
#include <lionheart/grammar.h>  // for MC6800_GRAMMAR
#include <lionheart/parser.h>   // for Parser
#include <lionheart/util.h>     // for is_variant
#include <string>               // for basic_string
#include <string_view>          // for basic_string_view

lionheart::assembly::Instruction test_at(
    std::variant<std::string, lionheart::assembly::Instruction> const&
        instruction)
{
    return std::get<lionheart::assembly::Instruction>(instruction);
}

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
    REQUIRE_EQ(symbols.at("REDIS").type, assembly::Symbol::Type::Variable);
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
    auto& instructions = parser.instructions();
    auto& symbols = parser.symbol_table();
    REQUIRE_NOTHROW(parser.parse(test));

    REQUIRE(instructions.size() == 1);
    REQUIRE(is_variant(instructions.at(0), std::string));
    REQUIRE_EQ(symbols.at("MYLABEL").type, assembly::Symbol::Type::Label);
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

    REQUIRE(test_at(instructions.at(0)).mnemonic == assembly::Mnemonic::ORG);
    REQUIRE(test_at(instructions.at(1)).mnemonic == assembly::Mnemonic::JSR);
    REQUIRE(test_at(instructions.at(2)).mnemonic == assembly::Mnemonic::LDA);
    REQUIRE(test_at(instructions.at(3)).mnemonic == assembly::Mnemonic::BRA);
    REQUIRE(test_at(instructions.at(4)).mnemonic == assembly::Mnemonic::TAB);
    REQUIRE(test_at(instructions.at(5)).mnemonic == assembly::Mnemonic::TST);

    REQUIRE(test_at(instructions.at(0)).mode == assembly::Mode::Directive);
    REQUIRE(test_at(instructions.at(1)).mode == assembly::Mode::Jump);
    REQUIRE(test_at(instructions.at(2)).mode == assembly::Mode::AccSrc8);
    REQUIRE(test_at(instructions.at(3)).mode == assembly::Mode::Branch);
    REQUIRE(test_at(instructions.at(4)).mode == assembly::Mode::Inherent);
    REQUIRE(test_at(instructions.at(5)).mode == assembly::Mode::Unary8);

    REQUIRE(test_at(instructions.at(0)).operands.size() == 1);
    REQUIRE(test_at(instructions.at(0)).operands.front() == "$F000");

    REQUIRE(test_at(instructions.at(1)).operands.size() == 1);
    REQUIRE(test_at(instructions.at(1)).operands.front() == "REDIS");

    REQUIRE(test_at(instructions.at(2)).operands.size() == 2);
    REQUIRE(test_at(instructions.at(2)).operands.front() == "A");
    REQUIRE(test_at(instructions.at(2)).operands.back() == "#$01");

    REQUIRE(test_at(instructions.at(3)).operands.size() == 1);
    REQUIRE(test_at(instructions.at(3)).operands.front() == "OUT");

    REQUIRE(test_at(instructions.at(4)).operands.size() == 0);

    REQUIRE(test_at(instructions.at(5)).operands.size() == 1);
    REQUIRE(test_at(instructions.at(5)).operands.front() == "B");
}