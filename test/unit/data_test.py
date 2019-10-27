"""
    axel is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    axel is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with axel.  If not, see <https://www.gnu.org/licenses/>.
"""
import pytest
from typing import List
from axel.symbol import U_Int8
from axel.assembler import Registers
from axel.tokens import AddressingMode
from axel.parser import Parser, AssemblerParserError
from axel.opcode import Translate
from axel.data import *

@pytest.fixture(scope='module')
def registers() -> Registers:
    Registers.AccA = U_Int8(0)
    Registers.AccB = U_Int8(0)
    yield Registers

@pytest.fixture
def parser() -> Parser:
    def _get_parser(source):
        parser = Parser(source)
        return parser

    return _get_parser

@pytest.fixture
def addr_codes() -> List[str]:
    return ['''SAME	LDA B  #$F0	; FIX DISPLAY ADDRESS
            ASL A
            ''',
        '      LDA B #$F0',
        '''; This is a comment\nADD B #$10
        ''',
        '''ADC A #$10
            ADC A $10
            ADD A $10,X
            BGE $FE
            BIT B $FCBC
            DAA
            DAA X''']


def test_processing_metafunction(parser, addr_codes, registers) -> None:
    r = registers()
    parser = Parser(addr_codes[3])
    _, operands = parser.line()
    test = processing(Translate.aba,
                      AddressingMode.ACC,
                      operands,
                      r)
    # test carry status
    r.AccA = U_Int8(5)
    r.AccB = U_Int8(255)
    do = test(AddressingMode.ACC,
               operands,
               r)
    assert r.SR[0] == True
    # test zero status
    r.AccA = U_Int8(0)
    r.AccB = U_Int8(0)
    do = test(AddressingMode.ACC,
               operands,
               r)
    assert r.SR[1] == True
    # test sign status
    r.AccA = U_Int8(-2)
    r.AccB = U_Int8(0)
    do = test(AddressingMode.ACC,
               operands,
               r)
    assert r.SR[2] == True
    # test overflow status
    r.AccA = U_Int8(-2)
    r.AccB = U_Int8(0)
    do = test(AddressingMode.ACC,
               operands,
               r)
    assert r.SR[3] == True


def test_get_addressing_mode(parser, addr_codes) -> None:
    parser = Parser(addr_codes[3])
    _, operands = parser.line()
    assert get_addressing_mode(parser, operands) == AddressingMode.IMM
    _, operands = parser.line()
    assert get_addressing_mode(parser, operands) == AddressingMode.DIR
    _, operands = parser.line()
    assert get_addressing_mode(parser, operands) == AddressingMode.IDX
    _, operands = parser.line()
    assert get_addressing_mode(parser, operands) == AddressingMode.REL
    _, operands = parser.line()
    assert get_addressing_mode(parser, operands) == AddressingMode.EXT
    _, operands = parser.line()
    assert get_addressing_mode(parser, operands) == AddressingMode.INH

def test_operand_state_machine(parser, addr_codes) -> None:
    parser = Parser(addr_codes[3])
    _, operands = parser.line()
    assert operand_state_machine(parser, operands, []) == AddressingMode.IMM
    _, operands = parser.line()
    assert operand_state_machine(parser, operands, []) == AddressingMode.DIR
    _, operands = parser.line()
    assert operand_state_machine(parser, operands, []) == AddressingMode.IDX
    _, operands = parser.line()
    assert operand_state_machine(parser, operands, []) == AddressingMode.REL
    _, operands = parser.line()
    assert operand_state_machine(parser, operands, []) == AddressingMode.EXT
    _, operands = parser.line()
    assert operand_state_machine(parser, operands, []) == AddressingMode.INH
    with pytest.raises(AssemblerParserError):
        _, operands = parser.line()
