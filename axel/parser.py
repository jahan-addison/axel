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
from typing import Union, List, overload
from axel.lexer import Lexer, Yylex
from axel.symbol import Symbol_Table, U_Int16

token_t = Union[Tokens.Token, Tokens.Mnemonic, Tokens.Register]


class Parser:
    def __init__(self, source: str) -> None:
        self.addressing_mode: Tokens.AddressingMode
        self.lexer: Lexer = Lexer(source)
        self.table: Symbol_Table = Symbol_Table()

    def parse_symbols(self) -> None:
        pass

    def parse(self) -> None:
        pass

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
                lexer.recede()
                raise SyntaxError(f'Parser failed near "{error}", '
                                  f'''expected one of "{','.join(options)}" '''
                                  f'got "{next_token.name}"')
        else:
            if next_token is not test:
                lexer.recede()
                raise SyntaxError(f'Parser failed near "{error}", '
                                  f'expected "{test.name}" '
                                  f'got "{next_token.name}"')

    def line(self) -> None:
        pass

    def variable(self, label: Yylex, addr: int) -> None:
        name = label['data']
        self.take(Tokens.Token.T_EQUAL)
        self.take([Tokens.Token.T_DIR_ADDR_UINT8,
                   Tokens.Token.T_EXT_ADDR_UINT16])
        if isinstance(name, str) and self.lexer.yylex['data'] is not None:
            self.table.set(
                name,
                U_Int16(addr),
                'variable',
                self.lexer.yylex['data'])
        else:
            raise SyntaxError(f'Parser failed to parse variable "{name}"')

    def label(self) -> None:
        pass

    def operand(self) -> None:
        pass

    def instruction(self) -> None:
        pass
