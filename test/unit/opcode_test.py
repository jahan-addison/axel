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
from axel.opcode import *

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

def test_opcode_aba(parser, registers) -> None:
    parser = Parser('ABA\n')
    instruction, operands = parser.line()
    r = registers()
    r.AccA = U_Int8(5)
    r.AccB = U_Int8(10)
    assert Translate.aba(AddressingMode.ACC, operands, r) == b'\x1b'
    assert r.AccA.num == 15