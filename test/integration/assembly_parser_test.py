"""
    Axel is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Axel is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Axel.  If not, see <https://www.gnu.org/licenses/>.
"""
import pytest
import pathlib
from collections import deque
from axel.tokens import Lexeme as Token, Mnemonic, Register
from axel.parser import Parser

@pytest.fixture
def parser() -> Parser:
    def _get_parser(source):
        return Parser(source)

    return _get_parser

expected = deque([
    (Mnemonic.T_JSR, Token.T_DISP_ADDR_INT8),
    (Mnemonic.T_LDA, Register.T_A, Token.T_IMM_UINT8),
    (Mnemonic.T_BRA, Token.T_DISP_ADDR_INT8),
    (Mnemonic.T_LDA, Register.T_B, Token.T_DISP_ADDR_INT8)
])

symbols = deque([
    'REDIS',
    'DIGADD',
    'OUTCH',
    'START',
    'SAME'
])

def test_assembly_parser(parser) -> None:
    with open(f'{pathlib.Path(__file__).parent.parent}/etc/fixture.asm') as f:
        test = parser(f.read())
        assert test.line() == True  # fills symbol table
        assert test.line() == True  # fills symbol table
        assert test.line() == True  # fills symbol table
        line = test.line()
        # test instruction parsing
        while line is not False:
            expect = expected.popleft()
            instruction, operands = line
            assert instruction == expect[0]
            operands.reverse()
            index = 1
            for ops in operands:
                assert ops['token'] == expect[index]
                index += 1

            line = test.line()

        # test symbol table construction
        assert len(test.symbols.table) == len(symbols)
        while len(symbols):
            assert symbols.pop() in test.symbols.table