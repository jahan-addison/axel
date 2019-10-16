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
from typing import Deque, Union
from axel.symbol import U_Int16, U_Int8, Symbol_Table
from axel.parser import Parser
from bitarray import bitarray

program: BytesIO = BytesIO()
program_stack: Deque[Union[int, str]] = deque()


class Registers:
    AccA: U_Int8 = U_Int8(0)
    AccB: U_Int8 = U_Int8(0)
    X: U_Int16 = U_Int16(0)
    SP: U_Int16 = U_Int16(0)
    PC: U_Int16 = U_Int16(0)
    SR: bitarray = bitarray([False] * 6)


def construct_symbol_table(source: str) -> Symbol_Table:
    parse = Parser(source)
    line = parse.line()
    while line:
        line = parse.line()
    return parse.symbols
