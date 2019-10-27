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
import types
from typing import Deque, Dict, Any
from axel.tokens import AddressingMode
from axel.lexer import yylex_t
from axel.data import processing
from axel.assembler import Registers as Register_T  # get class type


class Processor(type):
    def __new__(cls, name: str, bases: Any, attr: Dict[Any, Any]) -> Any:
        for name, value in attr.items():
            if not isinstance(value, types.FunctionType) \
                    and not isinstance(value, str):
                attr[name] = processing(value.__func__)

        return super(Processor, cls).__new__(cls, name, bases, attr)


class Translate(metaclass=Processor):
    @staticmethod
    def aba(addr_mode: AddressingMode,
            operands: Deque[yylex_t],
            registers: Register_T) -> bytearray:
        opcode = bytearray.fromhex('1B')
        if addr_mode == AddressingMode.ACC:
            registers.AccA += registers.AccB.num
        return opcode
