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

#include <lionheart/assembler.h>

#include <filesystem>           // for file_size, path
#include <fstream>              // for basic_ofstream, basic_ifstream, basi...
#include <lionheart/assembly.h> // for Instruction, get_accumulator, Addres...
#include <lionheart/byte.h>     // for mnemonic_addressing_mode_to_bytecode
#include <lionheart/memory.h>   // for strip_data_prefix, u16bit_safe_bytes...
#include <lionheart/util.h>     // for is_variant, overload
#include <matchit.h>            // for App, pattern, app, PatternHelper
#include <variant>              // for get, visit

#include <fmt/ranges.h>

namespace lionheart {

namespace m = matchit;

/**
 * @brief Emit the assembled binary output to a file
 */
void Assembler::emit(std::string const& filename)
{
    if (layout_.min_used > layout_.max_used) {
        throw Assembler_Error("empty input file", 0);
    }
    std::ofstream file(filename, std::ios::binary);
    if (file) {
        size_t output_size = layout_.max_used - layout_.min_used + 1;

        const char* start_ptr = reinterpret_cast<const char*>(
            layout_.memory.data() + layout_.min_used);

        file.write(start_ptr, output_size);
    }
}

/**
 * @brief Encode the MC6800 instructions
 */
void Assembler::encode()
{
    ///////////////////////////////////////////////
    // First pass: encode mnemonic instructions //
    ///////////////////////////////////////////////
    for (auto const& instruction : instructions_) {
        // clang-format off
        std::visit(util::overload{
        [&](std::string const& label) {
            label_address_.emplace(
                label, layout_.current_address);
            displacement_.try_emplace(label);
            jump_.try_emplace(label);
            },
        [&](assembly::Instruction const& inst) {
            m::match(inst.mode)(
                m::pattern | assembly::Mode::Inherent =
                    [&] {
                        layout_.write(assembly::
                                mnemonic_addressing_mode_to_bytecode(
                                    inst.mnemonic,
                                    assembly::Accumulator::None,
                                    assembly::Addressing_Mode::
                                        Inherent));
                    },
                m::pattern | assembly::Mode::Directive =
                    [&] {
                        encode_from_mnemonic_directives(inst);
                    },
                m::pattern | m::_ =
                    [&] {
                        encode_from_mnemonic_instruction(inst);
                        encode_from_mnemonic_operands(
                            inst.operands, inst.line);
                    });
            } },
        instruction);
        index_++;
        // clang-format on
    }
    //////////////////////////////////////
    // Second pass: Resolve jump tables //
    //////////////////////////////////////
    resolve_relative_bytecode_addresses();
}

/**
 * @brief Encode directive instructions
 */
void Assembler::encode_from_mnemonic_directives(
    assembly::Instruction const& instruction)
{
    auto& operands = instruction.operands;
    m::match(instruction.mnemonic)(
        m::pattern | assembly::Mnemonic::ORG =
            [&] {
                layout_.set_org(assembly::hex_from_string(
                    assembly::strip_data_prefix(operands.front())));
            },
        m::pattern | assembly::Mnemonic::FDB =
            [&] {
                if (symbols_.contains(operands.front())) {
                    auto symbol = symbols_.at(operands.front());
                    if (symbol.type == assembly::Symbol::Type::Label) {
                        if (!jump_.contains(symbol.value))
                            jump_.try_emplace(symbol.value);
                        jump_.at(symbol.value).first = instruction.line;
                        jump_.at(symbol.value)
                            .second.emplace(layout_.current_address);
                        layout_.write(std::byte{ 0x0 });
                    } else
                        for (auto const& byte :
                            assembly::u16bit_safe_bytes_from_hex_string(
                                assembly::strip_data_prefix(symbol.value)))
                            layout_.write(byte);
                } else {
                    auto bytes = assembly::u16bit_safe_bytes_from_hex_string(
                        assembly::strip_data_prefix(operands.front()));
                    for (auto const& byte : bytes)
                        layout_.write(byte);
                }
            });
}

/**
 * @brief Iterate over instruction operands and push each valid bytecode operand
 * to the memory output
 */
void Assembler::encode_from_mnemonic_operands(
    assembly::Operands const& operands,
    std::size_t line)
{
    for (auto const& operand : operands) {
        if (is_bytecode_operand(operand))
            push_operand_bytecode(operand, line);
    }
}

/**
 * @brief Resolve operand tokens to its bytecode bytes
 */
void Assembler::push_operand_bytecode(operand_t operand, std::size_t line)
{
    try {
        if (symbols_.contains(operand.data())) {
            using std::get;
            auto instruction = instructions_.at(index_);
            auto symbol = symbols_.at(operand.data());
            if (symbol.type == assembly::Symbol::Type::Label) {
                // default to the relative branch table
                auto& relative_table = displacement_;
                // on absolute jumps, add the offset to the jump table
                if (is_variant(instruction, assembly::Instruction) and
                    (get<assembly::Instruction>(instruction).mnemonic ==
                            assembly::Mnemonic::JSR or
                        get<assembly::Instruction>(instruction).mnemonic ==
                            assembly::Mnemonic::JMP))
                    relative_table = jump_;

                if (!relative_table.contains(symbol.value))
                    relative_table.try_emplace(symbol.value);
                relative_table.at(symbol.value).first = line;
                relative_table.at(symbol.value)
                    .second.emplace(layout_.current_address);
                layout_.write(std::byte{ 0x0 });
            } else
                for (auto const& byte :
                    assembly::u16bit_safe_bytes_from_hex_string(
                        assembly::strip_data_prefix(symbol.value)))
                    layout_.write(byte);
        } else {
            for (auto const& byte : assembly::u16bit_safe_bytes_from_hex_string(
                     assembly::strip_data_prefix(operand)))
                layout_.write(byte);
        }
    } catch (std::invalid_argument const& e) {
        throw Assembler_Error(
            fmt::format("invalid mnemonic operand '{}', {}", operand, e.what()),
            line);
    }
}

/**
 * @brief Get signed relative byte offset for branch, jump instructions
 */
std::byte Assembler::get_relative_offset_address(uint16_t branch_address,
    uint16_t target_address,
    std::size_t line,
    bool jump)
{
    uint16_t next_pc = branch_address + 2;
    int32_t offset =
        static_cast<int32_t>(target_address) - static_cast<int32_t>(next_pc);

    if (!jump and (offset < -128 or offset > 127))
        throw Assembler_Error(fmt::format("branch out of bounds, offset: {} ",
                                  std::to_string(offset)),
            line);

    uint8_t offset_address = static_cast<uint8_t>(static_cast<int8_t>(offset));
    return static_cast<std::byte>(offset_address);
}

/**
 * @brief Resolve relative displacements in jumps in the bytecode
 */
void Assembler::resolve_relative_bytecode_addresses()
{
    // relative displacement jumps
    for (auto const& table : displacement_) {
        for (auto const& address : table.second.second) {
            if (label_address_.contains(table.first)) {
                layout_.memory.at(address) =
                    get_relative_offset_address(address,
                        label_address_.at(table.first),
                        table.second.first);
            }
        }
    }
    // absolute jumps
    for (auto const& table : jump_) {
        for (auto const& address : table.second.second) {
            if (label_address_.contains(table.first)) {
                layout_.memory.at(address) =
                    get_relative_offset_address(address,
                        label_address_.at(table.first),
                        table.second.first,
                        true);
            }
        }
    }
}

/**
 * @brief Replace variable symbol names with their value in the table
 */
void Assembler::resolve_variable_bytecode_addresses(
    assembly::Operands& operands)
{
    for (std::size_t i = 0; i < operands.size(); i++) {
        auto operand = operands.at(i);
        if (symbols_.contains(operand) and
            symbols_.at(operand).type == assembly::Symbol::Type::Variable) {
            operands[i] = symbols_.at(operand).value;
        }
    }
}

/**
 * @brief Encode addressing for an mnemonic instruction
 */
void Assembler::encode_from_mnemonic_instruction(
    assembly::Instruction const& instruction)
{
    auto operands = instruction.operands;
    resolve_variable_bytecode_addresses(operands);
    m::match(operands)(
        m::pattern | m::app(assembly::is_accumulator_mode, true) =
            [&] {
                layout_.write(assembly::mnemonic_addressing_mode_to_bytecode(
                    instruction.mnemonic,
                    assembly::get_accumulator(operands.front()),
                    assembly::Addressing_Mode::Accumulator));
            },
        m::pattern | m::app(assembly::is_indexed_mode, true) =
            [&] {
                layout_.write(assembly::mnemonic_addressing_mode_to_bytecode(
                    instruction.mnemonic,
                    assembly::get_accumulator(operands.front()),
                    assembly::Addressing_Mode::Indexed));
            },
        m::pattern | m::app(assembly::is_extended_mode, true) =
            [&] {
                layout_.write(assembly::mnemonic_addressing_mode_to_bytecode(
                    instruction.mnemonic,
                    assembly::get_accumulator(operands.front()),
                    assembly::Addressing_Mode::Extended));
            },
        m::pattern | m::app(assembly::is_direct_mode, true) =
            [&] {
                layout_.write(assembly::mnemonic_addressing_mode_to_bytecode(
                    instruction.mnemonic,
                    assembly::get_accumulator(operands.front()),
                    assembly::Addressing_Mode::Direct));
            },
        m::pattern | m::app(assembly::is_immediate_mode, true) =
            [&] {
                layout_.write(assembly::mnemonic_addressing_mode_to_bytecode(
                    instruction.mnemonic,
                    assembly::get_accumulator(operands.front()),
                    assembly::Addressing_Mode::Immediate));
            },
        m::pattern |
            m::_ = [&] { encode_from_mnemonic_branch_jump(instruction); }

    );
}

/**
 * @brief Encode the opcode for absolute jump and relative branch
 */
void Assembler::encode_from_mnemonic_branch_jump(
    assembly::Instruction const& instruction)
{
    auto& operands = instruction.operands;
    // absolute jumps
    if (assembly::is_absolute_jump_instruction(instruction.mnemonic)) {
        if (assembly::is_indexed_mode(operands))
            layout_.write(assembly::mnemonic_addressing_mode_to_bytecode(
                instruction.mnemonic,
                assembly::get_accumulator(operands.front()),
                assembly::Addressing_Mode::Indexed));
        else
            layout_.write(assembly::mnemonic_addressing_mode_to_bytecode(
                instruction.mnemonic,
                assembly::get_accumulator(operands.front()),
                assembly::Addressing_Mode::Extended));
        return;
    }
    // branch jumps
    if (instruction.mode == assembly::Mode::Branch) {
        layout_.write(
            assembly::mnemonic_addressing_mode_to_bytecode(instruction.mnemonic,
                assembly::Accumulator::None,
                assembly::Addressing_Mode::Relative));
    } else if (operands.size() == 1 and symbols_.contains(operands.front()) and
               symbols_.at(operands.front()).type ==
                   assembly::Symbol::Type::Label) {
        layout_.write(
            assembly::mnemonic_addressing_mode_to_bytecode(instruction.mnemonic,
                assembly::Accumulator::None,
                assembly::Addressing_Mode::Relative));
    } else {
        if (instruction.mode != assembly::Mode::Directive) {
            throw Assembler_Error(
                fmt::format("invalid symbol '{}'", operands.front()),
                instruction.line);
        }
    }
}

/**
 * @brief Read the entire contents of a file on disk into a string
 */
std::string read_file_from_path(std::string_view path)
{
    std::ifstream f(path.data(), std::ios::in | std::ios::binary);
    const auto sz = std::filesystem::file_size(path);

    std::string result(sz, '\0');
    f.read(result.data(), sz);

    return result;
}

} // namespace lionheart