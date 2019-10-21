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
from axel.opcode import Opcodes, get_addressing_mode, operand_state_machine


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

def test_opcode_aba(parser, registers) -> None:
    parser = Parser('ABA\n')
    instruction, operands = parser.line()
    r = registers()
    r.AccA = U_Int8(5)
    r.AccB = U_Int8(10)
    assert Opcodes.aba(AddressingMode.ACC, operands, r) == b'\x1b'
    assert r.AccA.num == 15