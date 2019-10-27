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
import types
from pampy import match, _
from bitarray import bitarray
from typing import Deque, Tuple, Union, List, Any, Callable
from axel.assembler import Registers as Register_T  # get class type
from axel.tokens import AddressingMode, Token, Register, TokenEnum
from axel.tokens import First_Operand_States, Second_Operand_States
from axel.symbol import U_Int8
from axel.lexer import yylex_t
from axel.parser import Parser


Mode_T = Union[AddressingMode, Tuple[str, str, TokenEnum]]


def processing(func: types.FunctionType,
               *args: Any,
               **kwargs: Any) -> Callable[[Any, Any], Any]:
    def set_from_register(word: U_Int8, status_register: bitarray) -> None:
        # carry flag
        if word.raw > 255:
            status_register[0] = True
        # sign and overflow flag
        if word.raw < 0:
            status_register[3] = True
            status_register[2] = True
        # zero flag
        if word.num == 0:
            status_register[1] = True

    def set_status(*args: Any, **kwargs: Any) -> Any:
        operands: Deque[yylex_t] = args[1]
        registers: Register_T = args[2]
        op = func(*args, **kwargs)

        # Reset status register
        registers.SR = bitarray([False] * 6)

        if len(operands) > 1 and operands[-1]['token'] in Register:
            head_op = operands[-1]['token']
            if head_op in Register:
                if head_op == Register.T_B:
                    set_from_register(registers.AccB, registers.SR)
                else:
                    set_from_register(registers.AccA, registers.SR)
        return op
    return set_status


def get_addressing_mode(parser: Parser,
                        operands: Deque[yylex_t]) -> AddressingMode:
    operands = operands.copy()
    return operand_state_machine(parser, operands, [])


def operand_state_machine(parser: Parser,
                           operands: Deque[yylex_t],
                           mode_stack: List[AddressingMode]) -> AddressingMode:
    size: int = len(operands)
    if size > 0:
        if size > 1 and operands[1]['token'] == Token.T_COMMA:
            del operands[1]
        test = operands[0]['token']
        mode: Mode_T = match(len(operands),

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
