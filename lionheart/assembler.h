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

#include <cstddef>            // for byte
#include <fmt/format.h>       // for format
#include <lionheart/mc6800.h> // for Instructions, Symbols
#include <stdexcept>          // for runtime_error
#include <string>             // for basic_string, string
#include <utility>            // for move
#include <vector>             // for vector

/**
 *  Start of program:
 *  LDS #$00FF ; Initialize stack pointer
 *  ...
 *  ...
 *  End of program:
 *
 *  --- THE HARDWARE VECTORS ---
        ORG $FFF8       ; 4. Jump to the exact end of memory

        FDB $0000       ; IRQ Vector (Not used right now)
        FDB $0000       ; SWI Vector (Not used right now)
        FDB $0000       ; NMI Vector (Not used right now)
        FDB START       ; RESET Vector: Points the CPU to $F000 on boot!
 *
 */

namespace lionheart {

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

class Assembler
{
  public:
    Assembler() = delete;
    Assembler(Assembler const&) = delete;
    Assembler& operator=(Assembler const&) = delete;
    explicit Assembler(mc6800::Symbols symbols,
        mc6800::Instructions instructions)
        : symbols_(std::move(symbols))
        , instructions_(std::move(instructions))
    {
    }

    using byte_stream_t = std::vector<std::byte>;

  public:
    void encode();
    void emit(std::string const& filename = "o.bin");

  private:
    void encode_from_mnemonic_instruction(
        mc6800::Instruction const& instruction);

  private:
    mc6800::Symbols symbols_;
    mc6800::Instructions instructions_;
    byte_stream_t byte_stream_{};
};

std::string read_file_from_path(std::string_view path);

} // namespace lionheart