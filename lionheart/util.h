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

#include <string> // for basic_string, string

#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)

#define WIDE_AND_STRINGIFY(x) L##x
#define WIDE_STRINGIFY(x) WIDE_AND_STRINGIFY(x)

#define LIONHEART_PUBLIC public
#ifdef LIONHEART_TEST
#define PRIVATE_UNLESS_TESTED public
#define PROTECTED_UNLESS_TESTED public

#else
#define PRIVATE_UNLESS_TESTED private
#define PROTECTED_UNLESS_TESTED protected
#endif

namespace lionheart::util {

constexpr std::string get_boundary_substr(std::string const& token,
    std::string const& search = " \t\r\n\v\f")
{
    size_t start = token.find_first_not_of(search);
    if (start == std::string_view::npos)
        return {};
    size_t end = token.find_first_of(search, start);
    if (end == std::string_view::npos) {
        return token.substr(start);
    }
    return token.substr(start, end - start);
}

} // namespace util