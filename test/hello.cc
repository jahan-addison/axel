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

#include <doctest/doctest.h> // for filloss, ResultBuilder, toStream, toSt...

#include <axel/hello.h> // for Greeter
#include <string>       // for basic_string
#include <string_view>  // for basic_string_view

TEST_CASE("hello.cc: Greeter::greet")
{
    auto test = axel::Greeter{};
    CHECK_EQ(test.greet("Bob"), "Hello, Bob!");
    CHECK_EQ(test.greet("Sarah"), "Howdy, Sarah!");
    CHECK_EQ(test.greet("John"), "Hi, John!");
}