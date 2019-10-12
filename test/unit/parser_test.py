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
from typing import List
import axel.tokens as Tokens  # Token, Mnemonic, Register
from axel.parser import Parser

@pytest.fixture
def parser() -> Parser:
    def _get_parser(source):
        return Parser(source)

    return _get_parser

@pytest.fixture
def code() -> List[str]:
    return ['''SAME	LDA B DIGADD+1	; FIX DISPLAY ADDRESS
            ADD B #$10
        ''',
        'LDA B DIGADD+1',
        '''    ; This is a comment
        ADD B #$10
        ''',
        '''LDA B DIGADD+1 ; This is a comment \nADD B #$10''']

def test_take(parser, code):
    test = parser(code[0])
    test.take(Tokens.Token.T_LABEL)
    with pytest.raises(SyntaxError):
        test.take(Tokens.Mnemonic.T_ABA)
    test.take(Tokens.Mnemonic.T_LDA)
    with pytest.raises(SyntaxError):
        test.take([Tokens.Token.T_EXT_ADDR_UINT16, Tokens.Token.T_IMM_UINT8])
    test.take([Tokens.Register.T_A, Tokens.Register.T_B])
    test.take([Tokens.Token.T_DISP_ADDR_INT8, Tokens.Token.T_EXT_ADDR_UINT16])

