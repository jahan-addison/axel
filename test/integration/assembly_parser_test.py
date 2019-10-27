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
import pathlib
from collections import deque
from axel.symbol import Symbol_Table
from axel.tokens import Token, Mnemonic, Register
from axel.lexer import Lexer
from axel.parser import Parser, AssemblerParserError

@pytest.fixture
def symbol_table() -> Symbol_Table:
    def _get_symbols(source):
        scanner = Lexer(source)
        for token in scanner:
            pass
        return scanner.symbols

    return _get_symbols

expected = deque([
    (Mnemonic.T_JSR, Token.T_EXT_ADDR_UINT16),
    (Mnemonic.T_LDA, Register.T_A, Token.T_IMM_UINT8),
    (Mnemonic.T_BRA, Token.T_DISP_ADDR_INT8),
    (Mnemonic.T_LDA, Register.T_B, Token.T_DIR_ADDR_UINT8)
])

def test_assembly_parser_error(symbol_table) -> None:
    # unknown token
    test = Parser('FAIL\nADD B #$10\n', symbol_table('FAIL\nADD B #$10\n'))
    with pytest.raises(AssemblerParserError):
        test.take(Mnemonic.T_ADD)
    # unexpected
    test = Parser('ADD B #$10\n', symbol_table('ADD B #$10\n'))
    with pytest.raises(AssemblerParserError):
        test.take(Token.T_VARIABLE)

def test_assembly_parser(symbol_table) -> None:
    with open(f'{pathlib.Path(__file__).parent.parent}/etc/fixture.asm') as f:
        source = f.read()
        test = Parser(source, symbol_table(source))
        assert test.line() == True  # variable
        assert test.line() == True  # variable
        assert test.line() == True  # variable
        # note: skips empty lines
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
