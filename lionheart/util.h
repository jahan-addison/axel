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

#include <array>       // for array
#include <string>      // for basic_string, string
#include <string_view> // for basic_string_view, string_view

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

#define is_variant(variant, T) std::holds_alternative<T>(variant)

namespace lionheart::util {

// The overload pattern
// (std::variant visitor)

template<class... Ts>
struct overload : Ts...
{
    using Ts::operator()...;
};
template<class... Ts>
overload(Ts...) -> overload<Ts...>;

/**
 * @brief Split a string view at the given index and return both halves as a
 * two-element array
 */
constexpr std::array<std::string_view, 2> split_string_by_nth(
    std::string_view str,
    size_t n)
{
    std::string_view first = str.substr(0, n);
    std::string_view second =
        (str.size() > n) ? str.substr(n, n) : std::string_view{};

    return { first, second };
}

/**
 * @brief Get the first whitespace-delimited token from a string, trimming
 * leading and trailing whitespace
 */
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