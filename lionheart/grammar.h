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

#include <string_view> // for basic_string_view, string_view

/***************************************************************************
 * See https://github.com/yhirose/cpp-peglib for details on grammar syntax
 ***************************************************************************/

namespace lionheart::grammar {

using grammar_t = std::string_view;

constexpr grammar_t MC6800_GRAMMAR = R"(
        Start <- Instruction*
        Instruction <- Variable / Label / Mnemonic

        # Note: ORG and FDB are program directives

        Mnemonic <-
            Inherent('ABA') / AccSrc8('ADC') / AccSrc8('ADD') / AccSrc8('AND') / Unary8('ASL') / Unary8('ASR') /
            Branch('BCC') / Branch('BCS') / Branch('BEQ') / Branch('BGE') / Branch('BGT') / Branch('BHI') /
            AccSrc8('BIT') / Branch('BLE') / Branch('BLS') / Branch('BLT') / Branch('BMI') / Branch('BNE') /
            Branch('BPL') / Branch('BRA') / Branch('BSR') / Branch('BVC') / Branch('BVS') / Inherent('CBA') /
            Inherent('CLC') / Inherent('CLI') / Unary8('CLR') / Inherent('CLV') / AccSrc8('CMP') / Unary8('COM') /
            Wide('CPX') / Inherent('DAA') / Unary8('DEC') / Inherent('DES') / Inherent('DEX') / AccSrc8('EOR') /
            Unary8('INC') / Inherent('INS') / Inherent('INX') / Jump('JMP') / Jump('JSR') / AccSrc8('LDA') /
            Wide('LDS') / Wide('LDX') / Unary8('LSR') / Unary8('NEG') / Inherent('NOP') / AccSrc8('ORA') /
            AccOp('PSH') / AccOp('PUL') / Unary8('ROL') / Unary8('ROR') / Inherent('RTI') / Inherent('RTS') /
            Inherent('SBA') / AccSrc8('SBC') / Inherent('SEC') / Inherent('SEI') / Inherent('SEV') / AccStore('STA') /
            Store('STS') / Store('STX') / AccSrc8('SUB') / Inherent('SWI') / Inherent('TAB') / Inherent('TAP') /
            Inherent('TBA') / Inherent('TPA') / Unary8('TST') / Inherent('TSX') / Inherent('TXS') / Inherent('WAI')
                / Directive('ORG')
                / Directive('FDB')

        Inherent(op) <- < op >

        AccSrc8(op) <- < op > Accumulator (data_8bit_immediate / indexed_address / data_16bit_direct / data_8bit_direct)

        Unary8(op) <- < op > (Accumulator / indexed_address / data_16bit_direct)

        Branch(op) <- < op > (data_8bit_direct / identifier)

        Wide(op) <- < op > (data_16bit_immediate / indexed_address / data_16bit_direct / data_8bit_direct)

        Directive(op) <- < op > (data_16bit_direct / data_8bit_direct / identifier)

        Jump(op) <- < op > (indexed_address / data_16bit_direct / identifier)

        AccStore(op) <- < op > Accumulator (indexed_address / data_16bit_direct / data_8bit_direct)

        Store(op) <- < op > (indexed_address / data_16bit_direct / data_8bit_direct)

        AccOp(op) <- < op > Accumulator

        Variable <- identifier '=' data_16bit_direct

        Label <- < identifier ':' >

        Accumulator <- < 'A' / 'B' >

        indexed_address <- data_8bit_direct ',' 'X'

        data_16bit_immediate <- < '#' (hex_immediate_16bit / octal_immediate_16bit) >

        data_16bit_direct <- < (hex_immediate_16bit / octal_immediate_16bit) >

        data_8bit_immediate <- < '#' (hex_immediate_8bit / octal_immediate_8bit) >

        data_8bit_direct <- < (hex_immediate_8bit / octal_immediate_8bit) >

        hex_immediate_8bit <- < '$' [0-9a-fA-F]{2} >

        octal_immediate_8bit <- < '@' ([0-1][0-7]{2} / [0-7]{1,2}) >

        identifier <- < [a-z_A-Z][a-z_A-Z0-9]* >

        hex_immediate_16bit <- < '$' [0-9a-fA-F]{4} >

        octal_immediate_16bit <- < '@' ([0-1][0-7]{5} / [0-7]{1,5}) >

        %word <- identifier
        %whitespace <- ( [ \t\r\n\f\v] / comment )*
        comment     <- ';' [^\n]*
)";

} // namespace grammar