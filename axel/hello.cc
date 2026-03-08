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

#include <axel/hello.h>

#include <fmt/format.h> // for format
#include <matchit.h>    // for pattern, _, PatternPipable
#include <string>       // for basic_string, string
#include <string_view>  // for basic_string_view, string_view

namespace axel {

std::string Greeter::greet(std::string_view person)
{
    namespace m = matchit;

    return m::match(person)(
        m::pattern | "Bob" = []() -> std::string { return "Hello, Bob!"; },
        m::pattern | "Sarah" = []() -> std::string { return "Howdy, Sarah!"; },
        // default case
        m::pattern | m::_ = [&, person]() -> std::string {
            return fmt::format("Hi, {}!", person);
        });
}

} // namespace axel