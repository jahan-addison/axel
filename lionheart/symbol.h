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

#include <array>            // for array
#include <deque>            // for deque
#include <lionheart/util.h> // for sv
#include <matchit.h>        // for match
#include <string>           // for basic_string

#define MNEMONIC_EXPANSION(x)                  \
    []() -> lionheart::symbol::Mnemonic {      \
        return lionheart::symbol::Mnemonic::x; \
    }

#pragma once

namespace lionheart::symbol {

using Operands = std::deque<std::string>;

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

enum class Mode
{
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
    WAI
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
        m::pattern | "WAI" = MNEMONIC_EXPANSION(WAI));
}

/************************************************************************
 * This enables syntax-directed translation on a single mnemonic action *
 ************************************************************************/

constexpr std::array<Mode, 72> addressing_mode = {
    Mode::Inherent, // ABA
    Mode::AccSrc8,  // ADC
    Mode::AccSrc8,  // ADD
    Mode::AccSrc8,  // AND
    Mode::Unary8,   // ASL
    Mode::Unary8,   // ASR
    Mode::Branch,   // BCC
    Mode::Branch,   // BCS
    Mode::Branch,   // BEQ
    Mode::Branch,   // BGE
    Mode::Branch,   // BGT
    Mode::Branch,   // BHI
    Mode::AccSrc8,  // BIT
    Mode::Branch,   // BLE
    Mode::Branch,   // BLS
    Mode::Branch,   // BLT
    Mode::Branch,   // BMI
    Mode::Branch,   // BNE
    Mode::Branch,   // BPL
    Mode::Branch,   // BRA
    Mode::Branch,   // BSR
    Mode::Branch,   // BVC
    Mode::Branch,   // BVS
    Mode::Inherent, // CBA
    Mode::Inherent, // CLC
    Mode::Inherent, // CLI
    Mode::Unary8,   // CLR
    Mode::Inherent, // CLV
    Mode::AccSrc8,  // CMP
    Mode::Unary8,   // COM
    Mode::Wide,     // CPX
    Mode::Inherent, // DAA
    Mode::Unary8,   // DEC
    Mode::Inherent, // DES
    Mode::Inherent, // DEX
    Mode::AccSrc8,  // EOR
    Mode::Unary8,   // INC
    Mode::Inherent, // INS
    Mode::Inherent, // INX
    Mode::Jump,     // JMP
    Mode::Jump,     // JSR
    Mode::AccSrc8,  // LDA
    Mode::Wide,     // LDS
    Mode::Wide,     // LDX
    Mode::Unary8,   // LSR
    Mode::Unary8,   // NEG
    Mode::Inherent, // NOP
    Mode::AccSrc8,  // ORA
    Mode::AccOp,    // PSH
    Mode::AccOp,    // PUL
    Mode::Unary8,   // ROL
    Mode::Unary8,   // ROR
    Mode::Inherent, // RTI
    Mode::Inherent, // RTS
    Mode::Inherent, // SBA
    Mode::AccSrc8,  // SBC
    Mode::Inherent, // SEC
    Mode::Inherent, // SEI
    Mode::Inherent, // SEV
    Mode::AccStore, // STA
    Mode::Store,    // STS
    Mode::Store,    // STX
    Mode::AccSrc8,  // SUB
    Mode::Inherent, // SWI
    Mode::Inherent, // TAB
    Mode::Inherent, // TAP
    Mode::Inherent, // TBA
    Mode::Inherent, // TPA
    Mode::Unary8,   // TST
    Mode::Inherent, // TSX
    Mode::Inherent, // TXS
    Mode::Inherent, // WAI
};

struct Instruction
{
    Mnemonic mnemonic;
    Mode mode;
    Operands operands;
};

} // namespace types