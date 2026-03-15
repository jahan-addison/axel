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

#include <cstddef>              // for byte
#include <cstddef>              // for size_t
#include <fmt/format.h>         // for format
#include <lionheart/assembly.h> // for Instructions, Symbols, is_direct_prefix
#include <lionheart/memory.h>   // for Memory
#include <set>                  // for set
#include <stdexcept>            // for runtime_error
#include <stdint.h>             // for uint16_t, int16_t
#include <string>               // for basic_string, string
#include <string_view>          // for basic_string_view, string_view
#include <unordered_map>        // for unordered_map
#include <utility>              // for move

namespace lionheart {

/**
 * @brief
 *   Assembler Error Exception
 */
class Assembler_Error : public std::runtime_error
{
  public:
    Assembler_Error(std::string const& message, std::size_t line)
        : std::runtime_error(
              fmt::format(">>> Assembler Error:\n    on line {}, with: {}",
                  line,
                  message))
    {
    }
};

/**
 * @brief
 *   MC6800 Assembler
 */
class Assembler
{
  public:
    Assembler() = delete;
    Assembler(Assembler const&) = delete;
    Assembler& operator=(Assembler const&) = delete;
    explicit Assembler(assembly::Symbols symbols,
        assembly::Instructions instructions)
        : symbols_(std::move(symbols))
        , instructions_(std::move(instructions))
    {
    }
    using operand_t = std::string_view;

  public:
    void encode();
    void emit(std::string const& filename = "o.bin");

  private:
    void encode_from_mnemonic_instruction(
        assembly::Instruction const& instruction);
    void encode_from_mnemonic_directives(
        assembly::Instruction const& instruction);
    void encode_from_mnemonic_branch_jump(
        assembly::Instruction const& instruction);
    void encode_from_mnemonic_operands(assembly::Operands const& operands,
        std::size_t line);
    constexpr bool is_bytecode_operand(operand_t operand)
    {
        return assembly::is_immediate_prefix(operand) or
               assembly::is_direct_prefix(operand) or
               symbols_.contains(operand.data());
    }

  private:
    void push_operand_bytecode(operand_t operand, std::size_t line);
    void resolve_variable_bytecode_addresses(assembly::Operands& operands);
    void resolve_relative_bytecode_addresses();
    std::byte get_relative_offset_address(uint16_t branch_address,
        uint16_t target_address,
        std::size_t line,
        bool jump = false);

  private:
    assembly::Symbols symbols_;
    assembly::Instructions instructions_;

    // clang-format off
  PRIVATE_UNLESS_TESTED:
    assembly::Memory layout_{};
  private:
    using address_set_t = std::pair<std::size_t, std::set<std::size_t>>;
    using address_table_t = std::unordered_map<std::string, address_set_t>;
    Ordered_Map<std::string, int16_t> label_address_{};
    std::size_t index_{ 0 };
    address_table_t displacement_{};
    address_table_t jump_{};
};

std::string read_file_from_path(std::string_view path);

} // namespace lionheart