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

#include <doctest/doctest.h> // for ResultBuilder, TestCase, filloss, toS...

#include <fmt/base.h>          // for println
#include <lionheart/grammar.h> // for Grammar, MC6800_GRAMMAR
#include <lionheart/parser.h>  // for Parser, Symbol
#include <string>              // for basic_string
#include <string_view>         // for basic_string_view

TEST_CASE("Parser: Variable")
{
    using namespace lionheart;
    auto good = R"(
        REDIS   = $FE3A
    )";
    auto bad = R"(
        DIS     = $FE
        REDIS   = DIS
    )";

    auto parser = Parser{ grammar::MC6800_GRAMMAR };
    auto& symbols = parser.symbol_table();
    REQUIRE_NOTHROW(parser.parse(good));
    REQUIRE_EQ(symbols.at("REDIS").value, "$FE3A");
    REQUIRE_EQ(symbols.at("REDIS").type, detail::Symbol::Type::Variable);
    REQUIRE_THROWS(parser.parse(bad));
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
    REQUIRE_EQ(symbols.at("MYLABEL").type, detail::Symbol::Type::Label);
}