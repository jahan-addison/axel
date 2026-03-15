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

#include <algorithm>        // for max, min, __find_if, find_if
#include <array>            // for array
#include <cstddef>          // for byte
#include <cstdint>          // for uint16_t, uint8_t
#include <lionheart/util.h> // for split_string_by_nth
#include <stdexcept>        // for invalid_argument
#include <string_view>      // for basic_string_view, operator""sv, string...
#include <vector>           // for vector

namespace lionheart::assembly {

/**
 * @brief
 *  The entire universe of memory on the MC6800 is 64kb
 */
struct Memory
{
    std::vector<std::byte> memory{ 65536, std::byte{ 0xFF } };

    uint16_t current_address = 0x0000;

    uint16_t min_used = 0xFFFF;
    uint16_t max_used = 0x0000;

    inline void set_org(uint16_t new_address) { current_address = new_address; }

    inline void write(std::byte data)
    {
        memory.at(current_address) = data;

        min_used = std::min(min_used, current_address);
        max_used = std::max(max_used, current_address);

        current_address++;
    }
};

namespace {

/**
 * @brief Convert a single ASCII hex digit character to its 4-bit numeric value
 */
constexpr uint8_t parse_hex_char(const char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    throw std::invalid_argument("Invalid hex digit");
}

} // namespace

/**
 * @brief Strip any addressing prefix from the front of an operand
 */
constexpr std::string_view strip_data_prefix(std::string_view str)
{
    using namespace std::string_view_literals;

    constexpr std::array prefixes = { "#$"sv, "#@"sv, "#"sv, "$"sv, "@"sv };
    auto it = std::ranges::find_if(
        prefixes, [str](std::string_view p) { return str.starts_with(p); });
    if (it != prefixes.end()) {
        str.remove_prefix(it->size());
    }

    return str;
}

/**
 * @brief Parse a 1- or 2-character hex string into a single byte value
 */
constexpr std::byte byte_from_hex_string(std::string_view hex_string)
{
    if (hex_string.empty() || hex_string.size() > 2) {
        throw std::invalid_argument(
            "Hex byte string must be 1 or 2 characters");
    }

    uint8_t result = 0;
    for (char c : hex_string) {
        // cppcheck-suppress[useStlAlgorithm,knownEmptyContainer]
        result = static_cast<uint8_t>((result << 4) | parse_hex_char(c));
    }
    return static_cast<std::byte>(result);
}

/**
 * @brief Parse a hex string of up to 4 characters into a 16-bit unsigned
 * integer
 */
constexpr uint16_t hex_from_string(std::string_view hex_string)
{
    uint16_t result = 0;
    for (char c : hex_string) {
        // cppcheck-suppress[useStlAlgorithm,knownEmptyContainer]
        result = static_cast<uint16_t>((result << 4) | parse_hex_char(c));
    }
    return result;
}

/**
 * @brief Split a hex string into a vector of 1 or 2 bytes, safe for both 8-bit
 * and 16-bit values
 */
constexpr std::vector<std::byte> u16bit_safe_bytes_from_hex_string(
    std::string_view hex_string)
{
    std::vector<std::byte> bytes{};
    if (hex_string.empty() || hex_string.size() > 4) {
        throw std::invalid_argument(
            "Hex byte string must be 1 or 2 characters");
    }
    auto u16bit_safe = util::split_string_by_nth(hex_string, 2);
    bytes.emplace_back(byte_from_hex_string(u16bit_safe.at(0)));
    if (!u16bit_safe.back().empty())
        bytes.emplace_back(byte_from_hex_string(u16bit_safe.at(1)));
    return bytes;
}

} // namespace lionheart