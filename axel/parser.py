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
import axel.tokens as Tokens
from collections import deque
from typing import Union, List, overload, Deque
from axel.lexer import Lexer, Yylex
from axel.symbol import Symbol_Table, U_Int16

token_t = Union[Tokens.Lexeme, Tokens.Mnemonic, Tokens.Register]


class Parser:
    def __init__(self, source: str) -> None:
        self.addressing_mode: Tokens.AddressingMode
        self.lexer: Lexer = Lexer(source)
        self.symbols: Symbol_Table = Symbol_Table()

    def _error(self,
               source: str,
               expected: str,
               found: Tokens.TokenEnum) -> None:
        raise SyntaxError(
            f'Parser failed near "{source}", '
            f'expected one of "{expected}" '
            f'got "{found.name}"')

    def parse(self) -> None:
        pass

    def parse_immediate_value(self, value: str) -> bytes:
        """TODO: Decimal, binary, single character. """
        if value[:1] == '#' and value[1:2] == '$':
            return bytes.fromhex(value[2:])
        else:
            return bytes.fromhex(value[1:])

    @overload
    def take(self, test: List[token_t]) -> None: ...

    @overload
    def take(self, test: token_t) -> None: ...

    def take(self, test: Union[token_t, List[token_t]]) -> None:
        lexer = self.lexer
        next_token = next(lexer)
        location = 0 if lexer._pointer - 5 < 0 else lexer._pointer - 5
        error = lexer._source[location:12]
        if isinstance(test, list):
            if next_token not in test:
                options = list(map(lambda x: x.name, test))
                lexer.retract()
                self._error(error, ','.join(options), next_token)
        else:
            if next_token is not test:
                lexer.retract()
                self._error(error, test.name, next_token)

    def line(self) -> None:
        pass

    def variable(self, label: Yylex) -> None:
        name = label['data']
        addr = self.lexer.last_addr
        self.take(Tokens.Lexeme.T_EQUAL)
        self.take([Tokens.Lexeme.T_DIR_ADDR_UINT8,
                   Tokens.Lexeme.T_EXT_ADDR_UINT16])
        if isinstance(name, str) and self.lexer.yylex['data'] is not None:
            self.symbols.set(
                name,
                U_Int16(addr),
                'variable',
                self.parse_immediate_value(self.lexer.yylex['data']))
        else:
            raise SyntaxError(f'Parser failed to parse variable "{name}"')

    def label(self, label: Yylex) -> None:
        name = label['data']
        addr = self.lexer.last_addr
        if isinstance(name, str):
            self.symbols.set(name, U_Int16(addr), 'label', U_Int16(addr))
        else:
            raise SyntaxError(f'Parser failed to parse label "{name}"')

    def operands(self) -> Deque[Yylex]:
        stack: Deque[Yylex] = deque()
        datatypes = [
            Tokens.Lexeme.T_IMM_UINT8,
            Tokens.Lexeme.T_IMM_UINT16,
            Tokens.Lexeme.T_DIR_ADDR_UINT8,
            Tokens.Lexeme.T_EXT_ADDR_UINT16,
            Tokens.Lexeme.T_DISP_ADDR_INT8,
        ]
        while True:
            try:
                self.take([
                    *list(Tokens.Register),
                    Tokens.Lexeme.T_COMMA,
                    *datatypes])
                stack.appendleft(self.lexer.yylex)
            except SyntaxError:
                self.lexer.retract()
                break
        return stack

    def instruction(self) -> None:
        pass
