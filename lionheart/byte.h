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

#include <array>              // for array
#include <cstddef>            // for byte
#include <lionheart/mc6800.h> // for Mnemonic, Symbol
#include <matchit.h>          // for match
#include <string>             // for basic_string

#define MC6800_BYTE(x) \
    std::byte          \
    {                  \
        x              \
    }

#define MATCH_BYTE(x) m::expr(MC6800_BYTE(x))

namespace lionheart::mc6800 {

constexpr std::byte mnemonic_and_addressing_mode_to_opcode(Mnemonic mnemonic,
    Accumulator acc,
    Addressing_Mode mode)
{
    namespace m = matchit;
    return m::match(mnemonic)(
        m::pattern | Mnemonic::ABA = MATCH_BYTE(0x1B),
        m::pattern | Mnemonic::ADC =
            [&] {
                return m::match(acc)(
                    m::pattern | Accumulator::A =
                        [&] {
                            return m::match(mode)(
                                m::pattern | Addressing_Mode::Immediate =
                                    MATCH_BYTE(0x89),
                                m::pattern | Addressing_Mode::Direct =
                                    MATCH_BYTE(0x99),
                                m::pattern | Addressing_Mode::Indexed =
                                    MATCH_BYTE(0xA9),
                                m::pattern | Addressing_Mode::Extended =
                                    MATCH_BYTE(0xB9),
                                m::pattern | m::_ = MATCH_BYTE());
                        },
                    m::pattern | Accumulator::B =
                        [&] {
                            return m::match(mode)(
                                m::pattern | Addressing_Mode::Immediate =
                                    MATCH_BYTE(0xC9),
                                m::pattern | Addressing_Mode::Direct =
                                    MATCH_BYTE(0xD9),
                                m::pattern | Addressing_Mode::Indexed =
                                    MATCH_BYTE(0xE9),
                                m::pattern | Addressing_Mode::Extended =
                                    MATCH_BYTE(0xF9),
                                m::pattern | m::_ = MATCH_BYTE());
                        },
                    m::pattern | m::_ = MATCH_BYTE());
            },
        m::pattern | Mnemonic::ADD =
            [&] {
                return m::match(acc)(
                    m::pattern | Accumulator::A =
                        [&] {
                            return m::match(mode)(
                                m::pattern | Addressing_Mode::Immediate =
                                    MATCH_BYTE(0x8B),
                                m::pattern | Addressing_Mode::Direct =
                                    MATCH_BYTE(0x9B),
                                m::pattern | Addressing_Mode::Indexed =
                                    MATCH_BYTE(0xAB),
                                m::pattern | Addressing_Mode::Extended =
                                    MATCH_BYTE(0xBB),
                                m::pattern | m::_ = MATCH_BYTE());
                        },
                    m::pattern | Accumulator::B =
                        [&] {
                            return m::match(mode)(
                                m::pattern | Addressing_Mode::Immediate =
                                    MATCH_BYTE(0xCB),
                                m::pattern | Addressing_Mode::Direct =
                                    MATCH_BYTE(0xDB),
                                m::pattern | Addressing_Mode::Indexed =
                                    MATCH_BYTE(0xEB),
                                m::pattern | Addressing_Mode::Extended =
                                    MATCH_BYTE(0xFB),
                                m::pattern | m::_ = MATCH_BYTE());
                        },
                    m::pattern | m::_ = MATCH_BYTE());
            },
        m::pattern | Mnemonic::AND =
            [&] {
                return m::match(acc)(
                    m::pattern | Accumulator::A =
                        [&] {
                            return m::match(mode)(
                                m::pattern | Addressing_Mode::Immediate =
                                    MATCH_BYTE(0x84),
                                m::pattern | Addressing_Mode::Direct =
                                    MATCH_BYTE(0x94),
                                m::pattern | Addressing_Mode::Indexed =
                                    MATCH_BYTE(0xA4),
                                m::pattern | Addressing_Mode::Extended =
                                    MATCH_BYTE(0xB4),
                                m::pattern | m::_ = MATCH_BYTE());
                        },
                    m::pattern | Accumulator::B =
                        [&] {
                            return m::match(mode)(
                                m::pattern | Addressing_Mode::Immediate =
                                    MATCH_BYTE(0xC4),
                                m::pattern | Addressing_Mode::Direct =
                                    MATCH_BYTE(0xD4),
                                m::pattern | Addressing_Mode::Indexed =
                                    MATCH_BYTE(0xE4),
                                m::pattern | Addressing_Mode::Extended =
                                    MATCH_BYTE(0xF4),
                                m::pattern | m::_ = MATCH_BYTE());
                        },
                    m::pattern | m::_ = MATCH_BYTE());
            },
        m::pattern | Mnemonic::ASL =
            [&] {
                return m::match(mode)(
                    m::pattern | Addressing_Mode::Accumulator =
                        [&] {
                            return m::match(acc)(
                                m::pattern | Accumulator::A = MATCH_BYTE(0x48),
                                m::pattern | Accumulator::B = MATCH_BYTE(0x58),
                                m::pattern | m::_ = MATCH_BYTE());
                        },
                    m::pattern | Addressing_Mode::Indexed = MATCH_BYTE(0x68),
                    m::pattern | Addressing_Mode::Extended = MATCH_BYTE(0x78),
                    m::pattern | m::_ = MATCH_BYTE());
            },
        m::pattern | Mnemonic::ASR =
            [&] {
                return m::match(mode)(
                    m::pattern | Addressing_Mode::Accumulator =
                        [&] {
                            return m::match(acc)(
                                m::pattern | Accumulator::A = MATCH_BYTE(0x47),
                                m::pattern | Accumulator::B = MATCH_BYTE(0x57),
                                m::pattern | m::_ = MATCH_BYTE());
                        },
                    m::pattern | Addressing_Mode::Indexed = MATCH_BYTE(0x67),
                    m::pattern | Addressing_Mode::Extended = MATCH_BYTE(0x77),
                    m::pattern | m::_ = MATCH_BYTE());
            },
        m::pattern | Mnemonic::BCC = MATCH_BYTE(0x24),
        m::pattern | Mnemonic::BCS = MATCH_BYTE(0x25),
        m::pattern | Mnemonic::BEQ = MATCH_BYTE(0x27),
        m::pattern | Mnemonic::BGE = MATCH_BYTE(0x2C),
        m::pattern | Mnemonic::BGT = MATCH_BYTE(0x2E),
        m::pattern | Mnemonic::BHI = MATCH_BYTE(0x22),
        m::pattern | Mnemonic::BIT =
            [&] {
                return m::match(acc)(
                    m::pattern | Accumulator::A =
                        [&] {
                            return m::match(mode)(
                                m::pattern | Addressing_Mode::Immediate =
                                    MATCH_BYTE(0x85),
                                m::pattern | Addressing_Mode::Direct =
                                    MATCH_BYTE(0x95),
                                m::pattern | Addressing_Mode::Indexed =
                                    MATCH_BYTE(0xA5),
                                m::pattern | Addressing_Mode::Extended =
                                    MATCH_BYTE(0xB5),
                                m::pattern | m::_ = MATCH_BYTE());
                        },
                    m::pattern | Accumulator::B =
                        [&] {
                            return m::match(mode)(
                                m::pattern | Addressing_Mode::Immediate =
                                    MATCH_BYTE(0xC5),
                                m::pattern | Addressing_Mode::Direct =
                                    MATCH_BYTE(0xD5),
                                m::pattern | Addressing_Mode::Indexed =
                                    MATCH_BYTE(0xE5),
                                m::pattern | Addressing_Mode::Extended =
                                    MATCH_BYTE(0xF5),
                                m::pattern | m::_ = MATCH_BYTE());
                        },
                    m::pattern | m::_ = MATCH_BYTE());
            },
        m::pattern | Mnemonic::BLE = MATCH_BYTE(0x2F),
        m::pattern | Mnemonic::BLS = MATCH_BYTE(0x23),
        m::pattern | Mnemonic::BLT = MATCH_BYTE(0x2D),
        m::pattern | Mnemonic::BMI = MATCH_BYTE(0x2B),
        m::pattern | Mnemonic::BNE = MATCH_BYTE(0x26),
        m::pattern | Mnemonic::BPL = MATCH_BYTE(0x2A),
        m::pattern | Mnemonic::BRA = MATCH_BYTE(0x20),
        m::pattern | Mnemonic::BSR = MATCH_BYTE(0x8D),
        m::pattern | Mnemonic::BVC = MATCH_BYTE(0x28),
        m::pattern | Mnemonic::BVS = MATCH_BYTE(0x29),
        m::pattern | Mnemonic::CBA = MATCH_BYTE(0x11),
        m::pattern | Mnemonic::CLC = MATCH_BYTE(0x0C),
        m::pattern | Mnemonic::CLI = MATCH_BYTE(0x0E),
        m::pattern | Mnemonic::CLR =
            [&] {
                return m::match(mode)(
                    m::pattern | Addressing_Mode::Accumulator =
                        [&] {
                            return m::match(acc)(
                                m::pattern | Accumulator::A = MATCH_BYTE(0x4F),
                                m::pattern | Accumulator::B = MATCH_BYTE(0x5F),
                                m::pattern | m::_ = MATCH_BYTE());
                        },
                    m::pattern | Addressing_Mode::Indexed = MATCH_BYTE(0x6F),
                    m::pattern | Addressing_Mode::Extended = MATCH_BYTE(0x7F),
                    m::pattern | m::_ = MATCH_BYTE());
            },
        m::pattern | Mnemonic::CLV = MATCH_BYTE(0x0A),
        m::pattern | Mnemonic::CMP =
            [&] {
                return m::match(acc)(
                    m::pattern | Accumulator::A =
                        [&] {
                            return m::match(mode)(
                                m::pattern | Addressing_Mode::Immediate =
                                    MATCH_BYTE(0x81),
                                m::pattern | Addressing_Mode::Direct =
                                    MATCH_BYTE(0x91),
                                m::pattern | Addressing_Mode::Indexed =
                                    MATCH_BYTE(0xA1),
                                m::pattern | Addressing_Mode::Extended =
                                    MATCH_BYTE(0xB1),
                                m::pattern | m::_ = MATCH_BYTE());
                        },
                    m::pattern | Accumulator::B =
                        [&] {
                            return m::match(mode)(
                                m::pattern | Addressing_Mode::Immediate =
                                    MATCH_BYTE(0xC1),
                                m::pattern | Addressing_Mode::Direct =
                                    MATCH_BYTE(0xD1),
                                m::pattern | Addressing_Mode::Indexed =
                                    MATCH_BYTE(0xE1),
                                m::pattern | Addressing_Mode::Extended =
                                    MATCH_BYTE(0xF1),
                                m::pattern | m::_ = MATCH_BYTE());
                        },
                    m::pattern | m::_ = MATCH_BYTE());
            },
        m::pattern | Mnemonic::COM =
            [&] {
                return m::match(mode)(
                    m::pattern | Addressing_Mode::Accumulator =
                        [&] {
                            return m::match(acc)(
                                m::pattern | Accumulator::A = MATCH_BYTE(0x43),
                                m::pattern | Accumulator::B = MATCH_BYTE(0x53),
                                m::pattern | m::_ = MATCH_BYTE());
                        },
                    m::pattern | Addressing_Mode::Indexed = MATCH_BYTE(0x63),
                    m::pattern | Addressing_Mode::Extended = MATCH_BYTE(0x73),
                    m::pattern | m::_ = MATCH_BYTE());
            },
        m::pattern | Mnemonic::CPX =
            [&] {
                return m::match(mode)(
                    m::pattern | Addressing_Mode::Immediate = MATCH_BYTE(0x8C),
                    m::pattern | Addressing_Mode::Direct = MATCH_BYTE(0x9C),
                    m::pattern | Addressing_Mode::Indexed = MATCH_BYTE(0xAC),
                    m::pattern | Addressing_Mode::Extended = MATCH_BYTE(0xBC),
                    m::pattern | m::_ = MATCH_BYTE());
            },
        m::pattern | Mnemonic::DAA = MATCH_BYTE(0x19),
        m::pattern | Mnemonic::DEC =
            [&] {
                return m::match(mode)(
                    m::pattern | Addressing_Mode::Accumulator =
                        [&] {
                            return m::match(acc)(
                                m::pattern | Accumulator::A = MATCH_BYTE(0x4A),
                                m::pattern | Accumulator::B = MATCH_BYTE(0x5A),
                                m::pattern | m::_ = MATCH_BYTE());
                        },
                    m::pattern | Addressing_Mode::Indexed = MATCH_BYTE(0x6A),
                    m::pattern | Addressing_Mode::Extended = MATCH_BYTE(0x7A),
                    m::pattern | m::_ = MATCH_BYTE());
            },
        m::pattern | Mnemonic::DES = MATCH_BYTE(0x34),
        m::pattern | Mnemonic::DEX = MATCH_BYTE(0x09),
        m::pattern | Mnemonic::EOR =
            [&] {
                return m::match(acc)(
                    m::pattern | Accumulator::A =
                        [&] {
                            return m::match(mode)(
                                m::pattern | Addressing_Mode::Immediate =
                                    MATCH_BYTE(0x88),
                                m::pattern | Addressing_Mode::Direct =
                                    MATCH_BYTE(0x98),
                                m::pattern | Addressing_Mode::Indexed =
                                    MATCH_BYTE(0xA8),
                                m::pattern | Addressing_Mode::Extended =
                                    MATCH_BYTE(0xB8),
                                m::pattern | m::_ = MATCH_BYTE());
                        },
                    m::pattern | Accumulator::B =
                        [&] {
                            return m::match(mode)(
                                m::pattern | Addressing_Mode::Immediate =
                                    MATCH_BYTE(0xC8),
                                m::pattern | Addressing_Mode::Direct =
                                    MATCH_BYTE(0xD8),
                                m::pattern | Addressing_Mode::Indexed =
                                    MATCH_BYTE(0xE8),
                                m::pattern | Addressing_Mode::Extended =
                                    MATCH_BYTE(0xF8),
                                m::pattern | m::_ = MATCH_BYTE());
                        },
                    m::pattern | m::_ = MATCH_BYTE());
            },
        m::pattern | Mnemonic::INC =
            [&] {
                return m::match(mode)(
                    m::pattern | Addressing_Mode::Accumulator =
                        [&] {
                            return m::match(acc)(
                                m::pattern | Accumulator::A = MATCH_BYTE(0x4C),
                                m::pattern | Accumulator::B = MATCH_BYTE(0x5C),
                                m::pattern | m::_ = MATCH_BYTE());
                        },
                    m::pattern | Addressing_Mode::Indexed = MATCH_BYTE(0x6C),
                    m::pattern | Addressing_Mode::Extended = MATCH_BYTE(0x7C),
                    m::pattern | m::_ = MATCH_BYTE());
            },
        m::pattern | Mnemonic::INS = MATCH_BYTE(0x31),
        m::pattern | Mnemonic::INX = MATCH_BYTE(0x08),
        m::pattern | Mnemonic::JMP =
            [&] {
                return m::match(mode)(
                    m::pattern | Addressing_Mode::Indexed = MATCH_BYTE(0x6E),
                    m::pattern | Addressing_Mode::Extended = MATCH_BYTE(0x7E),
                    m::pattern | m::_ = MATCH_BYTE());
            },
        m::pattern | Mnemonic::JSR =
            [&] {
                return m::match(mode)(
                    m::pattern | Addressing_Mode::Indexed = MATCH_BYTE(0xAD),
                    m::pattern | Addressing_Mode::Extended = MATCH_BYTE(0xBD),
                    m::pattern | m::_ = MATCH_BYTE());
            },
        m::pattern | Mnemonic::LDA =
            [&] {
                return m::match(acc)(
                    m::pattern | Accumulator::A =
                        [&] {
                            return m::match(mode)(
                                m::pattern | Addressing_Mode::Immediate =
                                    MATCH_BYTE(0x86),
                                m::pattern | Addressing_Mode::Direct =
                                    MATCH_BYTE(0x96),
                                m::pattern | Addressing_Mode::Indexed =
                                    MATCH_BYTE(0xA6),
                                m::pattern | Addressing_Mode::Extended =
                                    MATCH_BYTE(0xB6),
                                m::pattern | m::_ = MATCH_BYTE());
                        },
                    m::pattern | Accumulator::B =
                        [&] {
                            return m::match(mode)(
                                m::pattern | Addressing_Mode::Immediate =
                                    MATCH_BYTE(0xC6),
                                m::pattern | Addressing_Mode::Direct =
                                    MATCH_BYTE(0xD6),
                                m::pattern | Addressing_Mode::Indexed =
                                    MATCH_BYTE(0xE6),
                                m::pattern | Addressing_Mode::Extended =
                                    MATCH_BYTE(0xF6),
                                m::pattern | m::_ = MATCH_BYTE());
                        },
                    m::pattern | m::_ = MATCH_BYTE());
            },
        m::pattern | Mnemonic::LDS =
            [&] {
                return m::match(mode)(
                    m::pattern | Addressing_Mode::Immediate = MATCH_BYTE(0x8E),
                    m::pattern | Addressing_Mode::Direct = MATCH_BYTE(0x9E),
                    m::pattern | Addressing_Mode::Indexed = MATCH_BYTE(0xAE),
                    m::pattern | Addressing_Mode::Extended = MATCH_BYTE(0xBE),
                    m::pattern | m::_ = MATCH_BYTE());
            },
        m::pattern | Mnemonic::LDX =
            [&] {
                return m::match(mode)(
                    m::pattern | Addressing_Mode::Immediate = MATCH_BYTE(0xCE),
                    m::pattern | Addressing_Mode::Direct = MATCH_BYTE(0xDE),
                    m::pattern | Addressing_Mode::Indexed = MATCH_BYTE(0xEE),
                    m::pattern | Addressing_Mode::Extended = MATCH_BYTE(0xFE),
                    m::pattern | m::_ = MATCH_BYTE());
            },
        m::pattern | Mnemonic::LSR =
            [&] {
                return m::match(mode)(
                    m::pattern | Addressing_Mode::Accumulator =
                        [&] {
                            return m::match(acc)(
                                m::pattern | Accumulator::A = MATCH_BYTE(0x44),
                                m::pattern | Accumulator::B = MATCH_BYTE(0x54),
                                m::pattern | m::_ = MATCH_BYTE());
                        },
                    m::pattern | Addressing_Mode::Indexed = MATCH_BYTE(0x64),
                    m::pattern | Addressing_Mode::Extended = MATCH_BYTE(0x74),
                    m::pattern | m::_ = MATCH_BYTE());
            },
        m::pattern | Mnemonic::NEG =
            [&] {
                return m::match(mode)(
                    m::pattern | Addressing_Mode::Accumulator =
                        [&] {
                            return m::match(acc)(
                                m::pattern | Accumulator::A = MATCH_BYTE(0x40),
                                m::pattern | Accumulator::B = MATCH_BYTE(0x50),
                                m::pattern | m::_ = MATCH_BYTE());
                        },
                    m::pattern | Addressing_Mode::Indexed = MATCH_BYTE(0x60),
                    m::pattern | Addressing_Mode::Extended = MATCH_BYTE(0x70),
                    m::pattern | m::_ = MATCH_BYTE());
            },
        m::pattern | Mnemonic::NOP = MATCH_BYTE(0x01),
        m::pattern | Mnemonic::ORA =
            [&] {
                return m::match(acc)(
                    m::pattern | Accumulator::A =
                        [&] {
                            return m::match(mode)(
                                m::pattern | Addressing_Mode::Immediate =
                                    MATCH_BYTE(0x8A),
                                m::pattern | Addressing_Mode::Direct =
                                    MATCH_BYTE(0x9A),
                                m::pattern | Addressing_Mode::Indexed =
                                    MATCH_BYTE(0xAA),
                                m::pattern | Addressing_Mode::Extended =
                                    MATCH_BYTE(0xBA),
                                m::pattern | m::_ = MATCH_BYTE());
                        },
                    m::pattern | Accumulator::B =
                        [&] {
                            return m::match(mode)(
                                m::pattern | Addressing_Mode::Immediate =
                                    MATCH_BYTE(0xCA),
                                m::pattern | Addressing_Mode::Direct =
                                    MATCH_BYTE(0xDA),
                                m::pattern | Addressing_Mode::Indexed =
                                    MATCH_BYTE(0xEA),
                                m::pattern | Addressing_Mode::Extended =
                                    MATCH_BYTE(0xFA),
                                m::pattern | m::_ = MATCH_BYTE());
                        },
                    m::pattern | m::_ = MATCH_BYTE());
            },
        m::pattern | Mnemonic::PSH =
            [&] {
                return m::match(acc)(
                    m::pattern | Accumulator::A = MATCH_BYTE(0x36),
                    m::pattern | Accumulator::B = MATCH_BYTE(0x37),
                    m::pattern | m::_ = MATCH_BYTE());
            },
        m::pattern | Mnemonic::PUL =
            [&] {
                return m::match(acc)(
                    m::pattern | Accumulator::A = MATCH_BYTE(0x32),
                    m::pattern | Accumulator::B = MATCH_BYTE(0x33),
                    m::pattern | m::_ = MATCH_BYTE());
            },
        m::pattern | Mnemonic::ROL =
            [&] {
                return m::match(mode)(
                    m::pattern | Addressing_Mode::Accumulator =
                        [&] {
                            return m::match(acc)(
                                m::pattern | Accumulator::A = MATCH_BYTE(0x49),
                                m::pattern | Accumulator::B = MATCH_BYTE(0x59),
                                m::pattern | m::_ = MATCH_BYTE());
                        },
                    m::pattern | Addressing_Mode::Indexed = MATCH_BYTE(0x69),
                    m::pattern | Addressing_Mode::Extended = MATCH_BYTE(0x79),
                    m::pattern | m::_ = MATCH_BYTE());
            },
        m::pattern | Mnemonic::ROR =
            [&] {
                return m::match(mode)(
                    m::pattern | Addressing_Mode::Accumulator =
                        [&] {
                            return m::match(acc)(
                                m::pattern | Accumulator::A = MATCH_BYTE(0x46),
                                m::pattern | Accumulator::B = MATCH_BYTE(0x56),
                                m::pattern | m::_ = MATCH_BYTE());
                        },
                    m::pattern | Addressing_Mode::Indexed = MATCH_BYTE(0x66),
                    m::pattern | Addressing_Mode::Extended = MATCH_BYTE(0x76),
                    m::pattern | m::_ = MATCH_BYTE());
            },
        m::pattern | Mnemonic::RTI = MATCH_BYTE(0x3B),
        m::pattern | Mnemonic::RTS = MATCH_BYTE(0x39),
        m::pattern | Mnemonic::SBA = MATCH_BYTE(0x10),
        m::pattern | Mnemonic::SBC =
            [&] {
                return m::match(acc)(
                    m::pattern | Accumulator::A =
                        [&] {
                            return m::match(mode)(
                                m::pattern | Addressing_Mode::Immediate =
                                    MATCH_BYTE(0x82),
                                m::pattern | Addressing_Mode::Direct =
                                    MATCH_BYTE(0x92),
                                m::pattern | Addressing_Mode::Indexed =
                                    MATCH_BYTE(0xA2),
                                m::pattern | Addressing_Mode::Extended =
                                    MATCH_BYTE(0xB2),
                                m::pattern | m::_ = MATCH_BYTE());
                        },
                    m::pattern | Accumulator::B =
                        [&] {
                            return m::match(mode)(
                                m::pattern | Addressing_Mode::Immediate =
                                    MATCH_BYTE(0xC2),
                                m::pattern | Addressing_Mode::Direct =
                                    MATCH_BYTE(0xD2),
                                m::pattern | Addressing_Mode::Indexed =
                                    MATCH_BYTE(0xE2),
                                m::pattern | Addressing_Mode::Extended =
                                    MATCH_BYTE(0xF2),
                                m::pattern | m::_ = MATCH_BYTE());
                        },
                    m::pattern | m::_ = MATCH_BYTE());
            },
        m::pattern | Mnemonic::SEC = MATCH_BYTE(0x0D),
        m::pattern | Mnemonic::SEI = MATCH_BYTE(0x0F),
        m::pattern | Mnemonic::SEV = MATCH_BYTE(0x0B),
        m::pattern | Mnemonic::STA =
            [&] {
                return m::match(acc)(
                    m::pattern | Accumulator::A =
                        [&] {
                            return m::match(mode)(
                                m::pattern | Addressing_Mode::Direct =
                                    MATCH_BYTE(0x97),
                                m::pattern | Addressing_Mode::Indexed =
                                    MATCH_BYTE(0xA7),
                                m::pattern | Addressing_Mode::Extended =
                                    MATCH_BYTE(0xB7),
                                m::pattern | m::_ = MATCH_BYTE());
                        },
                    m::pattern | Accumulator::B =
                        [&] {
                            return m::match(mode)(
                                m::pattern | Addressing_Mode::Direct =
                                    MATCH_BYTE(0xD7),
                                m::pattern | Addressing_Mode::Indexed =
                                    MATCH_BYTE(0xE7),
                                m::pattern | Addressing_Mode::Extended =
                                    MATCH_BYTE(0xF7),
                                m::pattern | m::_ = MATCH_BYTE());
                        },
                    m::pattern | m::_ = MATCH_BYTE());
            },
        m::pattern | Mnemonic::STS =
            [&] {
                return m::match(mode)(
                    m::pattern | Addressing_Mode::Direct = MATCH_BYTE(0x9F),
                    m::pattern | Addressing_Mode::Indexed = MATCH_BYTE(0xAF),
                    m::pattern | Addressing_Mode::Extended = MATCH_BYTE(0xBF),
                    m::pattern | m::_ = MATCH_BYTE());
            },
        m::pattern | Mnemonic::STX =
            [&] {
                return m::match(mode)(
                    m::pattern | Addressing_Mode::Direct = MATCH_BYTE(0xDF),
                    m::pattern | Addressing_Mode::Indexed = MATCH_BYTE(0xEF),
                    m::pattern | Addressing_Mode::Extended = MATCH_BYTE(0xFF),
                    m::pattern | m::_ = MATCH_BYTE());
            },
        m::pattern | Mnemonic::SUB =
            [&] {
                return m::match(acc)(
                    m::pattern | Accumulator::A =
                        [&] {
                            return m::match(mode)(
                                m::pattern | Addressing_Mode::Immediate =
                                    MATCH_BYTE(0x80),
                                m::pattern | Addressing_Mode::Direct =
                                    MATCH_BYTE(0x90),
                                m::pattern | Addressing_Mode::Indexed =
                                    MATCH_BYTE(0xA0),
                                m::pattern | Addressing_Mode::Extended =
                                    MATCH_BYTE(0xB0),
                                m::pattern | m::_ = MATCH_BYTE());
                        },
                    m::pattern | Accumulator::B =
                        [&] {
                            return m::match(mode)(
                                m::pattern | Addressing_Mode::Immediate =
                                    MATCH_BYTE(0xC0),
                                m::pattern | Addressing_Mode::Direct =
                                    MATCH_BYTE(0xD0),
                                m::pattern | Addressing_Mode::Indexed =
                                    MATCH_BYTE(0xE0),
                                m::pattern | Addressing_Mode::Extended =
                                    MATCH_BYTE(0xF0),
                                m::pattern | m::_ = MATCH_BYTE());
                        },
                    m::pattern | m::_ = MATCH_BYTE());
            },
        m::pattern | Mnemonic::SWI = MATCH_BYTE(0x3F),
        m::pattern | Mnemonic::TAB = MATCH_BYTE(0x16),
        m::pattern | Mnemonic::TAP = MATCH_BYTE(0x06),
        m::pattern | Mnemonic::TBA = MATCH_BYTE(0x17),
        m::pattern | Mnemonic::TPA = MATCH_BYTE(0x07),
        m::pattern | Mnemonic::TST =
            [&] {
                return m::match(mode)(
                    m::pattern | Addressing_Mode::Accumulator =
                        [&] {
                            return m::match(acc)(
                                m::pattern | Accumulator::A = MATCH_BYTE(0x4D),
                                m::pattern | Accumulator::B = MATCH_BYTE(0x5D),
                                m::pattern | m::_ = MATCH_BYTE());
                        },
                    m::pattern | Addressing_Mode::Indexed = MATCH_BYTE(0x6D),
                    m::pattern | Addressing_Mode::Extended = MATCH_BYTE(0x7D),
                    m::pattern | m::_ = MATCH_BYTE());
            },
        m::pattern | Mnemonic::TSX = MATCH_BYTE(0x30),
        m::pattern | Mnemonic::TXS = MATCH_BYTE(0x35),
        m::pattern | Mnemonic::WAI = MATCH_BYTE(0x3E),
        m::pattern | m::_ = MATCH_BYTE());
}

} // namespace mc6800
