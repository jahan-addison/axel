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

#include <filesystem>         // for file_size, path
#include <fstream>            // for basic_ofstream, basic_ios, ios, ofstream
#include <lionheart/byte.h>   // for mnemonic_and_addressing_mode_to_opcode
#include <lionheart/mc6800.h> // for Instruction, get_accumulator, Addressi...
#include <matchit.h>          // for App, pattern, app, Wildcard, PatternHe...

#include <fmt/base.h>

namespace lionheart {

namespace m = matchit;

void Assembler::emit(std::string const& filename)
{
    std::ofstream file(filename, std::ios::binary);
    if (file)
        file.write(reinterpret_cast<const char*>(byte_stream_.data()),
            byte_stream_.size());
}

void Assembler::encode()
{
    for (auto const& instruction : instructions_) {
        m::match(instruction.mode)(
            m::pattern | mc6800::Mode::Inherent =
                [&] {
                    byte_stream_.emplace_back(
                        mc6800::mnemonic_and_addressing_mode_to_opcode(
                            instruction.mnemonic,
                            mc6800::Accumulator::None,
                            mc6800::Addressing_Mode::Inherent));
                },
            m::pattern |
                m::_ = [&] { encode_from_mnemonic_instruction(instruction); });
    }
}

void Assembler::encode_from_mnemonic_instruction(
    mc6800::Instruction const& instruction)
{
    auto& operands = instruction.operands;
    m::match(operands)(
        m::pattern | m::app(mc6800::is_accumulator_mode, true) =
            [&] {
                byte_stream_.emplace_back(
                    mc6800::mnemonic_and_addressing_mode_to_opcode(
                        instruction.mnemonic,
                        mc6800::get_accumulator(operands.front()),
                        mc6800::Addressing_Mode::Accumulator));
            },
        m::pattern | m::app(mc6800::is_indexed_mode, true) =
            [&] {
                byte_stream_.emplace_back(
                    mc6800::mnemonic_and_addressing_mode_to_opcode(
                        instruction.mnemonic,
                        mc6800::get_accumulator(operands.front()),
                        mc6800::Addressing_Mode::Indexed));
            },
        m::pattern | m::app(mc6800::is_extended_mode, true) =
            [&] {
                byte_stream_.emplace_back(
                    mc6800::mnemonic_and_addressing_mode_to_opcode(
                        instruction.mnemonic,
                        mc6800::get_accumulator(operands.front()),
                        mc6800::Addressing_Mode::Extended));
            },
        m::pattern | m::app(mc6800::is_direct_mode, true) =
            [&] {
                byte_stream_.emplace_back(
                    mc6800::mnemonic_and_addressing_mode_to_opcode(
                        instruction.mnemonic,
                        mc6800::get_accumulator(operands.front()),
                        mc6800::Addressing_Mode::Direct));
            },
        m::pattern | m::app(mc6800::is_immediate_mode, true) =
            [&] {
                byte_stream_.emplace_back(
                    mc6800::mnemonic_and_addressing_mode_to_opcode(
                        instruction.mnemonic,
                        mc6800::get_accumulator(operands.front()),
                        mc6800::Addressing_Mode::Immediate));
            },
        m::pattern | m::_ =
            [&] {
                if (instruction.mode == mc6800::Mode::Branch) {
                    byte_stream_.emplace_back(
                        mc6800::mnemonic_and_addressing_mode_to_opcode(
                            instruction.mnemonic,
                            mc6800::Accumulator::None,
                            mc6800::Addressing_Mode::Relative));
                } else if (operands.size() == 1 and
                           symbols_.contains(operands.front()) and
                           symbols_.at(operands.front()).type ==
                               mc6800::Symbol::Type::Label) {
                    byte_stream_.emplace_back(
                        mc6800::mnemonic_and_addressing_mode_to_opcode(
                            instruction.mnemonic,
                            mc6800::Accumulator::None,
                            mc6800::Addressing_Mode::Relative));
                } else
                    throw Assembler_Error(
                        fmt::format("invalid symbol '{}'", operands.front()),
                        instruction.line);
            }

    );
}

std::string read_file_from_path(std::string_view path)
{
    std::ifstream f(path.data(), std::ios::in | std::ios::binary);
    const auto sz = std::filesystem::file_size(path);

    std::string result(sz, '\0');
    f.read(result.data(), sz);

    return result;
}

} // namespace lionheart