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
from axel.tokens import Lexeme as Token, Mnemonic, Register
from axel.lexer import Lexer

@pytest.fixture
def lexer() -> Lexer:
    def _make_lexer(source):
        return Lexer(source)

    return _make_lexer

expected = deque([
    Token.T_VARIABLE,
    Token.T_EQUAL,
    Token.T_EXT_ADDR_UINT16,
    Token.T_VARIABLE,
    Token.T_EQUAL,
    Token.T_DIR_ADDR_UINT8,
    Token.T_VARIABLE,
    Token.T_EQUAL,
    Token.T_EXT_ADDR_UINT16,
    Token.T_LABEL,
    Mnemonic.T_JSR,
    Token.T_DISP_ADDR_INT8,
    Mnemonic.T_LDA,
    Register.T_A,
    Token.T_IMM_UINT8,
    Mnemonic.T_BRA,
    Token.T_DISP_ADDR_INT8,
    Token.T_LABEL,
    Mnemonic.T_LDA,
    Register.T_B,
    Token.T_DISP_ADDR_INT8
])

def test_lexer_tokenization() -> None:
    with open(f'{pathlib.Path(__file__).parent.parent}/etc/fixture.asm') as f:
        test = Lexer(f.read())
        for token in test:
           assert token is expected.popleft()
        assert len(expected) == 0
