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

#include <doctest/doctest.h> // for TestCase, ResultBuilder, REQUIRE, TEST_...

#include <axel/grammar.h> // for Grammar
#include <string>         // for basic_string

TEST_CASE("grammar: grammar expansion")
{
    auto grammar = axel::grammar::Grammar{};

    REQUIRE(grammar.rule_expansion(true) ==
            "MNEMONIC VARIABLE OPERANDS LABEL OPERAND REGISTER "
            "data_16bit_immediate data_16bit_direct data_8bit_immediate "
            "data_8bit_direct displacement hex_immediate_8bit "
            "octal_immediate_8bit hex_immediate_16bit octal_immediate_16bit");
}