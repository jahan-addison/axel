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
from typing import List, Callable, Any
from axel.symbol import U_Int8
from axel.assembler import Registers
from axel.tokens import AddressingMode
from axel.parser import Parser, AssemblerParserError
from axel.opcode import Translate
from axel.data import processing, get_addressing_mode, operand_state_machine

f1_t = Callable[[], Any]
f2_t = Callable[[str], Parser]
f3_t = List[str]


@pytest.fixture(scope='module')  # type: ignore
def registers() -> Callable[[], Any]:
    Registers.AccA = U_Int8(0)
    Registers.AccB = U_Int8(0)
    yield Registers


@pytest.fixture  # type: ignore
def parser() -> Callable[[str], Parser]:
    def _get_parser(source: str) -> Parser:
        parser = Parser(source)
        return parser

    return _get_parser


@pytest.fixture  # type: ignore
def addr_codes() -> List[str]:
    return [
        '''SAME	LDA B  #$F0	; FIX DISPLAY ADDRESS
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


def test_processing_metafunction(parser: f2_t,
                                 addr_codes: f3_t,
                                 registers: f1_t) -> None:
    r = registers()
    parse = parser(addr_codes[3])
    line = parse.line()
    if isinstance(line, bool):
        raise AssertionError('line is bool')
    _, operands = line
    test = processing(Translate.aba,  # type: ignore
                      AddressingMode.ACC,
                      operands,
                      r)
    # test carry status
    r.AccA = U_Int8(5)
    r.AccB = U_Int8(255)
    test(AddressingMode.ACC,  # type: ignore
         operands,
         r)
    assert r.SR[0] is True
    # test zero status
    r.AccA = U_Int8(0)
    r.AccB = U_Int8(0)
    test(AddressingMode.ACC,  # type: ignore
         operands,
         r)
    assert r.SR[1] is True
    # test sign status
    r.AccA = U_Int8(-2)
    r.AccB = U_Int8(0)
    test(AddressingMode.ACC,  # type: ignore
         operands,
         r)
    assert r.SR[2] is True
    # test overflow status
    r.AccA = U_Int8(-2)
    r.AccB = U_Int8(0)
    test(AddressingMode.ACC,  # type: ignore
         operands,
         r)
    assert r.SR[3] is True


def test_get_addressing_mode(parser: f2_t, addr_codes: f3_t) -> None:
    test: Parser = parser(addr_codes[3])
    _, operands = test.line()  # type: ignore
    assert get_addressing_mode(test, operands) == AddressingMode.IMM
    _, operands = test.line()  # type: ignore
    assert get_addressing_mode(test, operands) == AddressingMode.DIR
    _, operands = test.line()  # type: ignore
    assert get_addressing_mode(test, operands) == AddressingMode.IDX
    _, operands = test.line()  # type: ignore
    assert get_addressing_mode(test, operands) == AddressingMode.REL
    _, operands = test.line()  # type: ignore
    assert get_addressing_mode(test, operands) == AddressingMode.EXT
    _, operands = test.line()  # type: ignore
    assert get_addressing_mode(test, operands) == AddressingMode.INH


def test_operand_state_machine(parser: f2_t, addr_codes: f3_t) -> None:
    test = parser(addr_codes[3])
    _, operands = test.line()  # type: ignore
    assert operand_state_machine(test, operands, []) == AddressingMode.IMM
    _, operands = test.line()  # type: ignore
    assert operand_state_machine(test, operands, []) == AddressingMode.DIR
    _, operands = test.line()  # type: ignore
    assert operand_state_machine(test, operands, []) == AddressingMode.IDX
    _, operands = test.line()  # type: ignore
    assert operand_state_machine(test, operands, []) == AddressingMode.REL
    _, operands = test.line()  # type: ignore
    assert operand_state_machine(test, operands, []) == AddressingMode.EXT
    _, operands = test.line()  # type: ignore
    assert operand_state_machine(test, operands, []) == AddressingMode.INH
    with pytest.raises(AssemblerParserError):
        _, operands = test.line()   # type: ignore
