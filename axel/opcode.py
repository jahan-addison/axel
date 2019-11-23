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
from bitarray import bitarray
from axel.tokens import AddressingMode
from axel.parser import Parser, AssemblerParserError
from axel.lexer import yylex_t
from axel.data import processing
from axel.assembler import Registers as Register_T  # get class type


class Processor(type):
    """The Processor metaclass.

    Decorates each `Translate` staticmethod with pre- and post-processing
    for the accumulator and status register flags in each instruction.
    """
    def __new__(cls, name: str, bases: Any, attr: Dict[Any, Any]) -> Any:
        for name, value in attr.items():
            if not isinstance(value, types.FunctionType) \
                    and not isinstance(value, str):
                attr[name] = processing(value.__func__)

        return super(Processor, cls).__new__(cls, name, bases, attr)


class Translate(metaclass=Processor):
    """The Opcode translation static class.

    Compiles each instruction operation per addressing mode, operands, and
    registers, returning a bytearray of the opcode.

    Runs post-processing on the accumulators and status register.
    """
    @staticmethod
    def aba(addr_mode: AddressingMode,
            operands: Deque[yylex_t],
            registers: Register_T) -> bytearray:
        """The ABA instruction. """
        opcode = bytearray.fromhex('1B')
        if addr_mode == AddressingMode.ACC:
            registers.AccA += registers.AccB.num
        return opcode

    @staticmethod
    def adc(addr_mode: AddressingMode,
            operands: Deque[yylex_t],
            registers: Register_T) -> bytearray:
        """The ADC instruction. """
        opcode: bytearray = bytearray()
        data: int = 0
        status: bitarray = registers.SR
        o = operands[0]['data']
        if not isinstance(o, str):
            raise AssemblerParserError(f'Invalid instruction operand')
        # Immediate addressing:
        if addr_mode == AddressingMode.IMM:
            opcode = bytearray.fromhex('89') if operands[-1]['data'] == 'A' \
                else bytearray.fromhex('C9')
            operand = int(Parser.parse_immediate_value(o).hex(), 16)
            if status[0] is True:
                b = bin(operand)
                data = int('0b' + status.to01()[0] + b[2:], 2)
            else:
                data = int(bin(operand), 2)
            registers.AccA += data
        return opcode + bytearray.fromhex(hex(data)[2:])
