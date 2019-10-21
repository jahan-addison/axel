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
# flake8: noqa
from pampy import match, _
from typing import Deque, Tuple, Union, Optional, List
from axel.tokens import AddressingMode, Token, Register, TokenEnum
from axel.tokens import First_Operand_States, Second_Operand_States
from axel.assembler import Registers as Register_T
from axel.lexer import Yylex
from axel.parser import Parser

Mode = Union[AddressingMode, Tuple[str, str, TokenEnum]]


class Opcodes():
    @staticmethod
    def aba(addr_mode: AddressingMode,
            operands: Deque[Yylex],
            registers: Register_T) -> bytearray:
        opcode = bytearray.fromhex('1B')
        if addr_mode == AddressingMode.INH:
            registers.AccA += registers.AccB.num
        return opcode


def get_addressing_mode(parser: Parser, operands: Deque[Yylex]) -> AddressingMode:
    operands = operands.copy()

    return operand_state_machine(parser, operands, [])


def operand_state_machine(parser: Parser,
                           operands: Deque[Yylex],
                           mode_stack: List[AddressingMode]) -> AddressingMode:
    size: int = len(operands)
    if size > 0:
        if size > 1 and operands[1]['token'] == Token.T_COMMA:
            del operands[1]
        test = operands[0]['token']
        mode: Mode = match(len(operands),

            3,  lambda k: AddressingMode.IDX if
                    operands[0]['token'] == Register.T_X
                    else ('error', Register.T_X.name, test),

            2,  lambda k: match(test,
                    Token.T_IMM_UINT8,         AddressingMode.IMM,
                    Token.T_IMM_UINT16,        AddressingMode.IMM,
                    Token.T_DIR_ADDR_UINT8,    AddressingMode.DIR,
                    Token.T_EXT_ADDR_UINT16,   AddressingMode.EXT,
                    Register.T_X,              AddressingMode.IDX,
                    _,                         ('error',
                                                ', '.join(list(map(lambda x: x.name,
                                                                   Second_Operand_States))),
                                                test)
                ),

            1,  lambda k: match(test,
                    Token.T_IMM_UINT16,       AddressingMode.IMM,
                    Token.T_DIR_ADDR_UINT8,   AddressingMode.DIR,
                    Token.T_DISP_ADDR_INT8,   AddressingMode.REL,
                    Token.T_EXT_ADDR_UINT16,  AddressingMode.EXT,
                    Register.T_A,             AddressingMode.ACC,
                    Register.T_B,             AddressingMode.ACC,
                    _,                        ('error',
                                               ', '.join(list(map(lambda x: x.name,
                                                    First_Operand_States))),
                                                test)
                ),

            0,  lambda k: AddressingMode.INH
        )
        if not isinstance(mode, AddressingMode):
            parser.error(mode[1], mode[2])
        else:
            operands.popleft()
            mode_stack.append(mode)

        return operand_state_machine(parser, operands, mode_stack)

    return AddressingMode.INH if len(mode_stack) == 0 else mode_stack[0]
