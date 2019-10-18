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
from axel.lexer import Yylex
from axel.parser import Parser

Mode = Union[AddressingMode, Tuple[str, str, TokenEnum]]

second_operand = [
    Token.T_IMM_UINT8,
    Token.T_IMM_UINT16,
    Token.T_DIR_ADDR_UINT8,
    Token.T_EXT_ADDR_UINT16,
    Register.T_X,
]

first_operand = [
    Token.T_IMM_UINT16,
    Token.T_DIR_ADDR_UINT8,
    Token.T_DISP_ADDR_INT8,
    Token.T_EXT_ADDR_UINT16,
    Register.T_A,
    Register.T_B
]


def get_addressing_mode(
        parser: Parser,
        operands: Deque[Yylex],
        mode_stack: List[AddressingMode]) -> Optional[AddressingMode]:
    operands = operands.copy()
    size = len(operands)
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
                                                ', '.join(list(map(lambda x: x.name, second_operand))),
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
                                                    first_operand))),
                                                test)
                ),

            0,  lambda k: AddressingMode.INH
        )
        if not isinstance(mode, AddressingMode):
            parser.error(mode[1], mode[2])
        else:
            operands.popleft()
            mode_stack.append(mode)
        return get_addressing_mode(parser, operands, mode_stack)
    else:
        return AddressingMode.INH if len(mode_stack) == 0 else mode_stack[0]

    return AddressingMode.INH if len(mode_stack) == 0 else mode_stack[0]
