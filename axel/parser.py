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
from typing import Union, List
from axel.lexer import Lexer
from axel.symbol import Symbol_Table


class Parser:
    def __init__(self, source: str) -> None:
        self.addressing_mode: Tokens.AddressingMode
        self.lexer: Lexer = Lexer(source)
        self.table: Symbol_Table = Symbol_Table()

    def parse_symbols(self) -> None:
        pass

    def parse(self) -> None:
        pass

    def take(self, oneOf: List[Union[Tokens.Token,
                               Tokens.Mnemonic,
                               Tokens.Register]]) -> None:
        next_token = next(self.lexer)
        if next_token not in oneOf:
            lexer = self.lexer
            location = 0 if lexer._pointer - 5 < 0 else lexer._pointer - 5
            error = lexer._source[location:12]
            raise SyntaxError(f'Parser failed near "{error.strip()}", '
                              f'expected one of "{oneOf}"')

    def line(self) -> None:
        pass

    def variable(self) -> None:
        pass

    def label(self) -> None:
        pass

    def operand(self) -> None:
        pass

    def instruction(self) -> None:
        pass
