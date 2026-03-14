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

#include <array>            // for array
#include <cstddef>          // for byte
#include <deque>            // for deque
#include <lionheart/map.h>  // for Ordered_Map
#include <lionheart/util.h> // for split_string_by_nth
#include <matchit.h>        // for match
#include <string>           // for basic_string
#include <vector>           // for vector

#define MNEMONIC_EXPANSION(x)                  \
    []() -> lionheart::mc6800::Mnemonic {      \
        return lionheart::mc6800::Mnemonic::x; \
    }

namespace lionheart::mc6800 {

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

using Operands = std::deque<std::string>;

struct PC
{
    u_int16_t location{ 0 };
};

struct Symbol
{
    enum class Type
    {
        Variable,
        Label
    };
    std::string value;
    Type type;
    std::size_t line;
};

/**
 * @brief
 * Relative mode:
 *  Signed two's complement, msb = 1 for negative jump
 *    * -126 to +129 byte limit
 */
enum class Mode
{
    Directive,
    Inherent,
    AccSrc8,
    Unary8,
    Branch,
    Wide,
    Jump,
    AccStore,
    Store,
    AccOp
};

enum class Addressing_Mode
{
    Accumulator,
    Immediate,
    Direct,
    Extended,
    Indexed,
    Inherent,
    Relative
};

enum class Accumulator
{
    A,
    B,
    None
};

enum class Mnemonic
{
    ABA,
    ADC,
    ADD,
    AND,
    ASL,
    ASR,
    BCC,
    BCS,
    BEQ,
    BGE,
    BGT,
    BHI,
    BIT,
    BLE,
    BLS,
    BLT,
    BMI,
    BNE,
    BPL,
    BRA,
    BSR,
    BVC,
    BVS,
    CBA,
    CLC,
    CLI,
    CLR,
    CLV,
    CMP,
    COM,
    CPX,
    DAA,
    DEC,
    DES,
    DEX,
    EOR,
    INC,
    INS,
    INX,
    JMP,
    JSR,
    LDA,
    LDS,
    LDX,
    LSR,
    NEG,
    NOP,
    ORA,
    PSH,
    PUL,
    ROL,
    ROR,
    RTI,
    RTS,
    SBA,
    SBC,
    SEC,
    SEI,
    SEV,
    STA,
    STS,
    STX,
    SUB,
    SWI,
    TAB,
    TAP,
    TBA,
    TPA,
    TST,
    TSX,
    TXS,
    WAI,

    ORG,
    FDB
};

constexpr Mnemonic mnemonic_string_index(std::string_view mnemonic)
{
    namespace m = matchit;
    return m::match(mnemonic)(m::pattern | "ABA" = MNEMONIC_EXPANSION(ABA),
        m::pattern | "ADC" = MNEMONIC_EXPANSION(ADC),
        m::pattern | "ADD" = MNEMONIC_EXPANSION(ADD),
        m::pattern | "AND" = MNEMONIC_EXPANSION(AND),
        m::pattern | "ASL" = MNEMONIC_EXPANSION(ASL),
        m::pattern | "ASR" = MNEMONIC_EXPANSION(ASR),
        m::pattern | "BCC" = MNEMONIC_EXPANSION(BCC),
        m::pattern | "BCS" = MNEMONIC_EXPANSION(BCS),
        m::pattern | "BEQ" = MNEMONIC_EXPANSION(BEQ),
        m::pattern | "BGE" = MNEMONIC_EXPANSION(BGE),
        m::pattern | "BGT" = MNEMONIC_EXPANSION(BGT),
        m::pattern | "BHI" = MNEMONIC_EXPANSION(BHI),
        m::pattern | "BIT" = MNEMONIC_EXPANSION(BIT),
        m::pattern | "BLE" = MNEMONIC_EXPANSION(BLE),
        m::pattern | "BLS" = MNEMONIC_EXPANSION(BLS),
        m::pattern | "BLT" = MNEMONIC_EXPANSION(BLT),
        m::pattern | "BMI" = MNEMONIC_EXPANSION(BMI),
        m::pattern | "BNE" = MNEMONIC_EXPANSION(BNE),
        m::pattern | "BPL" = MNEMONIC_EXPANSION(BPL),
        m::pattern | "BRA" = MNEMONIC_EXPANSION(BRA),
        m::pattern | "BSR" = MNEMONIC_EXPANSION(BSR),
        m::pattern | "BVC" = MNEMONIC_EXPANSION(BVC),
        m::pattern | "BVS" = MNEMONIC_EXPANSION(BVS),
        m::pattern | "CBA" = MNEMONIC_EXPANSION(CBA),
        m::pattern | "CLC" = MNEMONIC_EXPANSION(CLC),
        m::pattern | "CLI" = MNEMONIC_EXPANSION(CLI),
        m::pattern | "CLR" = MNEMONIC_EXPANSION(CLR),
        m::pattern | "CLV" = MNEMONIC_EXPANSION(CLV),
        m::pattern | "CMP" = MNEMONIC_EXPANSION(CMP),
        m::pattern | "COM" = MNEMONIC_EXPANSION(COM),
        m::pattern | "CPX" = MNEMONIC_EXPANSION(CPX),
        m::pattern | "DAA" = MNEMONIC_EXPANSION(DAA),
        m::pattern | "DEC" = MNEMONIC_EXPANSION(DEC),
        m::pattern | "DES" = MNEMONIC_EXPANSION(DES),
        m::pattern | "DEX" = MNEMONIC_EXPANSION(DEX),
        m::pattern | "EOR" = MNEMONIC_EXPANSION(EOR),
        m::pattern | "INC" = MNEMONIC_EXPANSION(INC),
        m::pattern | "INS" = MNEMONIC_EXPANSION(INS),
        m::pattern | "INX" = MNEMONIC_EXPANSION(INX),
        m::pattern | "JMP" = MNEMONIC_EXPANSION(JMP),
        m::pattern | "JSR" = MNEMONIC_EXPANSION(JSR),
        m::pattern | "LDA" = MNEMONIC_EXPANSION(LDA),
        m::pattern | "LDS" = MNEMONIC_EXPANSION(LDS),
        m::pattern | "LDX" = MNEMONIC_EXPANSION(LDX),
        m::pattern | "LSR" = MNEMONIC_EXPANSION(LSR),
        m::pattern | "NEG" = MNEMONIC_EXPANSION(NEG),
        m::pattern | "NOP" = MNEMONIC_EXPANSION(NOP),
        m::pattern | "ORA" = MNEMONIC_EXPANSION(ORA),
        m::pattern | "PSH" = MNEMONIC_EXPANSION(PSH),
        m::pattern | "PUL" = MNEMONIC_EXPANSION(PUL),
        m::pattern | "ROL" = MNEMONIC_EXPANSION(ROL),
        m::pattern | "ROR" = MNEMONIC_EXPANSION(ROR),
        m::pattern | "RTI" = MNEMONIC_EXPANSION(RTI),
        m::pattern | "RTS" = MNEMONIC_EXPANSION(RTS),
        m::pattern | "SBA" = MNEMONIC_EXPANSION(SBA),
        m::pattern | "SBC" = MNEMONIC_EXPANSION(SBC),
        m::pattern | "SEC" = MNEMONIC_EXPANSION(SEC),
        m::pattern | "SEI" = MNEMONIC_EXPANSION(SEI),
        m::pattern | "SEV" = MNEMONIC_EXPANSION(SEV),
        m::pattern | "STA" = MNEMONIC_EXPANSION(STA),
        m::pattern | "STS" = MNEMONIC_EXPANSION(STS),
        m::pattern | "STX" = MNEMONIC_EXPANSION(STX),
        m::pattern | "SUB" = MNEMONIC_EXPANSION(SUB),
        m::pattern | "SWI" = MNEMONIC_EXPANSION(SWI),
        m::pattern | "TAB" = MNEMONIC_EXPANSION(TAB),
        m::pattern | "TAP" = MNEMONIC_EXPANSION(TAP),
        m::pattern | "TBA" = MNEMONIC_EXPANSION(TBA),
        m::pattern | "TPA" = MNEMONIC_EXPANSION(TPA),
        m::pattern | "TST" = MNEMONIC_EXPANSION(TST),
        m::pattern | "TSX" = MNEMONIC_EXPANSION(TSX),
        m::pattern | "TXS" = MNEMONIC_EXPANSION(TXS),
        m::pattern | "WAI" = MNEMONIC_EXPANSION(WAI),
        m::pattern | "ORG" = MNEMONIC_EXPANSION(ORG),
        m::pattern | "FDB" = MNEMONIC_EXPANSION(ORG));
}

namespace {

constexpr std::initializer_list<std::string_view> byte_prefix = { "$", "@" };

} // namespace

constexpr bool is_direct_prefix(std::string_view search, std::size_t index = 0)
{
    return std::ranges::find(byte_prefix, search.substr(index, 1)) !=
           byte_prefix.end();
}

constexpr bool is_immediate_prefix(std::string_view search)
{
    return search.size() > 0 and search.at(0) == '#';
}

constexpr bool is_accumulator(std::string_view operand)
{
    return operand == "A" or operand == "B";
}

constexpr bool is_direct_8bit_operand(std::string_view operand)
{
    return is_direct_prefix(operand) and operand.size() == 3; // $FC
}

constexpr bool is_direct_16bit_operand(std::string_view operand)
{
    return is_direct_prefix(operand) and operand.size() == 5; // $F110
}

constexpr bool is_immediate_8bit_operand(std::string_view operand)
{
    return is_immediate_prefix(operand) and is_direct_prefix(operand, 1) and
           operand.size() == 4; // #$10
}

constexpr bool is_immediate_16bit_operand(std::string_view operand)
{
    return is_immediate_prefix(operand) and is_direct_prefix(operand, 1) and
           operand.size() == 6; // #$F010
}

constexpr Accumulator get_accumulator(std::string_view operand)
{
    if (operand != "A" and operand != "B")
        return Accumulator::None;
    else
        return operand == "B" ? Accumulator::B : Accumulator::A;
}

inline bool is_indexed_mode(Operands const& operands)
{
    return operands.size() == 3 and operands.at(2).ends_with("X");
}

inline bool is_accumulator_mode(Operands const& operands)
{
    return operands.size() == 1 and is_accumulator(operands.front());
}

inline bool is_direct_mode(Operands const& operands)
{
    if (operands.size() == 1)
        return is_direct_8bit_operand(operands.back());
    if (operands.size() == 2)
        return is_accumulator(operands.front()) and
               is_direct_8bit_operand(operands.back());
    else
        return false;
}

inline bool is_extended_mode(Operands const& operands)
{
    if (operands.size() == 1)
        return is_direct_16bit_operand(operands.back());
    else if (operands.size() == 2)
        return is_accumulator(operands.front()) and
               is_direct_16bit_operand(operands.back());
    else
        return false;
}

inline bool is_immediate_mode(Operands const& operands)
{
    if (operands.size() == 1)
        return is_immediate_8bit_operand(operands.back()) or
               is_immediate_16bit_operand(operands.back());
    else if (operands.size() == 2)
        return is_accumulator(operands.front()) and
               is_immediate_8bit_operand(operands.back());
    else
        return false;
}

namespace {

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

constexpr std::byte byte_from_hex_string(std::string_view hex_string)
{
    if (hex_string.empty() || hex_string.size() > 2) {
        throw std::invalid_argument(
            "Hex byte string must be 1 or 2 characters");
    }

    uint8_t result = 0;
    for (char c : hex_string) {
        result = static_cast<uint8_t>((result << 4) | parse_hex_char(c));
    }
    return static_cast<std::byte>(result);
}

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

/************************************************************************
 * Per-Instruction address modes
 *  This enables syntax-directed translation on a single mnemonic action
 ************************************************************************/

constexpr std::array<Mode, 74> addressing_mode = {
    Mode::Inherent,  // ABA
    Mode::AccSrc8,   // ADC
    Mode::AccSrc8,   // ADD
    Mode::AccSrc8,   // AND
    Mode::Unary8,    // ASL
    Mode::Unary8,    // ASR
    Mode::Branch,    // BCC
    Mode::Branch,    // BCS
    Mode::Branch,    // BEQ
    Mode::Branch,    // BGE
    Mode::Branch,    // BGT
    Mode::Branch,    // BHI
    Mode::AccSrc8,   // BIT
    Mode::Branch,    // BLE
    Mode::Branch,    // BLS
    Mode::Branch,    // BLT
    Mode::Branch,    // BMI
    Mode::Branch,    // BNE
    Mode::Branch,    // BPL
    Mode::Branch,    // BRA
    Mode::Branch,    // BSR
    Mode::Branch,    // BVC
    Mode::Branch,    // BVS
    Mode::Inherent,  // CBA
    Mode::Inherent,  // CLC
    Mode::Inherent,  // CLI
    Mode::Unary8,    // CLR
    Mode::Inherent,  // CLV
    Mode::AccSrc8,   // CMP
    Mode::Unary8,    // COM
    Mode::Wide,      // CPX
    Mode::Inherent,  // DAA
    Mode::Unary8,    // DEC
    Mode::Inherent,  // DES
    Mode::Inherent,  // DEX
    Mode::AccSrc8,   // EOR
    Mode::Unary8,    // INC
    Mode::Inherent,  // INS
    Mode::Inherent,  // INX
    Mode::Jump,      // JMP
    Mode::Jump,      // JSR
    Mode::AccSrc8,   // LDA
    Mode::Wide,      // LDS
    Mode::Wide,      // LDX
    Mode::Unary8,    // LSR
    Mode::Unary8,    // NEG
    Mode::Inherent,  // NOP
    Mode::AccSrc8,   // ORA
    Mode::AccOp,     // PSH
    Mode::AccOp,     // PUL
    Mode::Unary8,    // ROL
    Mode::Unary8,    // ROR
    Mode::Inherent,  // RTI
    Mode::Inherent,  // RTS
    Mode::Inherent,  // SBA
    Mode::AccSrc8,   // SBC
    Mode::Inherent,  // SEC
    Mode::Inherent,  // SEI
    Mode::Inherent,  // SEV
    Mode::AccStore,  // STA
    Mode::Store,     // STS
    Mode::Store,     // STX
    Mode::AccSrc8,   // SUB
    Mode::Inherent,  // SWI
    Mode::Inherent,  // TAB
    Mode::Inherent,  // TAP
    Mode::Inherent,  // TBA
    Mode::Inherent,  // TPA
    Mode::Unary8,    // TST
    Mode::Inherent,  // TSX
    Mode::Inherent,  // TXS
    Mode::Inherent,  // WAI
    Mode::Directive, // ORG
    Mode::Directive, // FDB
};

struct Instruction
{
    Mnemonic mnemonic;
    Mode mode;
    Operands operands;

    std::size_t line{ 0 };
};

using Instructions = std::deque<Instruction>;
using Symbols = Ordered_Map<std::string, Symbol>;

} // namespace mc6800