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
from io import BytesIO
from collections import deque
from typing import Deque, Union, Optional
from axel.symbol import Symbol_Table, U_Int8, U_Int16
from axel.lexer import Lexer
from axel.parser import Parser
from bitarray import bitarray

Stack = Deque[Union[int, str]]


class Registers:
    AccA: U_Int8 = U_Int8(0)
    AccB: U_Int8 = U_Int8(0)
    X: U_Int16 = U_Int16(0)
    SP: U_Int16 = U_Int16(0)
    PC: U_Int16 = U_Int16(0)
    SR: bitarray = bitarray([False] * 6)
    _stack: Stack = deque()


class Assembler:
    def __init__(self, source: str) -> None:
        self.lexer = Optional[Lexer]
        self.symbol_table: Symbol_Table = self._construct_symbol_table(
            source
        )
        self.parser: Parser = Parser(source, self.symbol_table)
        self.program: BytesIO = BytesIO()

    def _construct_symbol_table(self, source: str) -> Symbol_Table:
        self.lexer = Lexer(source)
        for token in self.lexer:
            pass
        return self.lexer.symbols

    def assemble(self) -> BytesIO:
        return self.program
