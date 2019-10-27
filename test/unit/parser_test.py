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
import axel.tokens as Tokens  # Token, Mnemonic, Register
from axel.lexer import Lexer
from axel.symbol import Symbol_Table, U_Int16
from axel.parser import *

@pytest.fixture
def parser() -> Parser:
    def _get_parser(source, symbols = Symbol_Table()):
        return Parser(source, symbols)

    return _get_parser

@pytest.fixture
def symbol_table() -> Symbol_Table:
    def _get_symbols(source):
        scanner = Lexer(source)
        for token in scanner:
            pass
        return scanner.symbols

    return _get_symbols

@pytest.fixture
def code() -> List[str]:
    return ['''SAME LDA B #$F0	; FIX DISPLAY ADDRESS
            ADD B #$10
        ''',
        '''OUTCH = $FE3A
           START JSR $FCBC	;SET UP FIRST DISPLAY ADDRESS
        ''',
        '''START JSR $FCBC	;SET UP FIRST DISPLAY ADDRESS''',
        '\n\nADD B #$10\n']

def test_take(parser, code) -> None:
    test = parser(code[0])
    test.take(Tokens.Token.T_LABEL)
    with pytest.raises(AssemblerParserError):
        test.take(Tokens.Mnemonic.T_ABA)
    test.take(Tokens.Mnemonic.T_LDA)
    with pytest.raises(AssemblerParserError):
        test.take([Tokens.Token.T_EXT_ADDR_UINT16, Tokens.Token.T_IMM_UINT8])
    test.take([Tokens.Register.T_A, Tokens.Register.T_B])
    test.take([Tokens.Token.T_IMM_UINT8])

def test_parse_immediate_value(parser, code) -> None:
    test = parser(code[0])
    assert test.parse_immediate_value('#$10') == b'\x10'
    assert test.parse_immediate_value('$10') == b'\x10'

def test_error(parser, code) -> None:
    test = parser(code[0])
    expected = 'Parser failed near "SAME LDA B #", expected one of T_LABEL, but found "T_EOL"'
    with pytest.raises(AssemblerParserError, match=expected) as error:
        test.error('T_LABEL', Tokens.Token.T_EOL)

def test_line(parser, code) -> None:
    test = parser(code[3])
    instruction, operands = test.line()
    assert instruction == Tokens.Mnemonic.T_ADD
    assert operands.pop()['token'] == Tokens.Register.T_B
    assert operands.pop()['token'] == Tokens.Token.T_IMM_UINT8
    assert len(operands) == 0


def test_variable(parser, code, symbol_table) -> None:
    test = parser(code[1], symbol_table(code[1]))
    next(test.lexer) # eat token
    test.variable(test.lexer.yylex)
    entry = test.symbols.table['OUTCH']
    assert entry[0].num == 0
    assert entry[1] == 'variable'
    assert entry[2] == b'\xfe:'
    assert test.lexer._pointer == 13

def test_operands(parser, code) -> None:
    test = parser(code[0])
    test.lexer._pointer = 9
    operands = test.operands()
    immediate, register = operands
    assert register['token'] == Tokens.Register.T_B
    assert immediate['token'] == Tokens.Token.T_IMM_UINT8

def test_instructions(parser, code) -> None:
    test = parser(code[0])
    test.lexer._pointer = 9
    test.lexer.yylex = {
        'token': Tokens.Mnemonic.T_LDA,
        'data': None
    }
    instruction, operands = test.instruction(test.lexer.yylex)
    immediate, register = operands
    assert instruction == Tokens.Mnemonic.T_LDA
    assert register['token'] == Tokens.Register.T_B
    assert immediate['token'] == Tokens.Token.T_IMM_UINT8
