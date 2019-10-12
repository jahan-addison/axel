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
from enum import Enum, unique, auto
from typing import TypeVar

T = TypeVar('T', bound='TokenEnum')


class TokenEnum(Enum):
    """Token Enumerations.

    Encapsulates useful details in each enum for quick access,
    while guaranteeing uniqueness on each `auto'.
    """
    def _generate_next_value_(  # type: ignore
            self: T,
            start: int,
            count: int,
            last_values: int) -> T:
        return self


@unique
class Token(TokenEnum):
    """Lexeme tokens.

    General language lexemes and data types.
    """
    T_LVALUE = auto()
    T_LABEL = auto()
    T_EQUAL = auto()
    T_COMMA = auto()
    T_REGISTER = auto()
    T_UNKNOWN = auto()
    T_IMM_UINT8 = auto()
    T_IMM_UINT16 = auto()
    T_DIR_ADDR_UINT8 = auto()
    T_EXT_ADDR_UINT16 = auto()
    T_DISP_ADDR_INT8 = auto()


@unique
class AddressingMode(TokenEnum):
    """Address modes. """
    # Acumulator addressing
    ACC = auto()
    # Immediate addressing
    IMM = auto()
    # Direct addressing
    DIR = auto()
    # Extended addressing
    EXT = auto()
    # Indexed addressing
    IDX = auto()
    # Implied (inherent) addressing
    INH = auto()
    # Relative addressing
    REL = auto()


@unique
class Register(TokenEnum):
    """Registers. """
    T_A = auto()
    T_B = auto()
    T_X = auto()
    T_PC = auto()
    T_SP = auto()
    T_SR = auto()


@unique
class Mnemonic(TokenEnum):
    """Mnemonics. """
    T_ABA = auto()
    T_ADC = auto()
    T_ADD = auto()
    T_AND = auto()
    T_ASL = auto()
    T_ASR = auto()
    T_BCC = auto()
    T_BCS = auto()
    T_BEQ = auto()
    T_BGE = auto()
    T_BGT = auto()
    T_BHI = auto()
    T_BIT = auto()
    T_BLE = auto()
    T_BLS = auto()
    T_BLT = auto()
    T_BMI = auto()
    T_BNE = auto()
    T_BPL = auto()
    T_BRA = auto()
    T_BSR = auto()
    T_BVC = auto()
    T_BVS = auto()
    T_CBA = auto()
    T_CLC = auto()
    T_CLI = auto()
    T_CLR = auto()
    T_CLV = auto()
    T_CMP = auto()
    T_COM = auto()
    T_CPX = auto()
    T_DAA = auto()
    T_DEC = auto()
    T_DES = auto()
    T_DEX = auto()
    T_EOR = auto()
    T_INC = auto()
    T_INS = auto()
    T_INX = auto()
    T_JMP = auto()
    T_JSR = auto()
    T_LDA = auto()
    T_LDS = auto()
    T_LDX = auto()
    T_LSR = auto()
    T_NEG = auto()
    T_NOP = auto()
    T_ORA = auto()
    T_PSH = auto()
    T_PUL = auto()
    T_ROL = auto()
    T_ROR = auto()
    T_RTI = auto()
    T_RTS = auto()
    T_SBA = auto()
    T_SBC = auto()
    T_SEC = auto()
    T_SEI = auto()
    T_SEV = auto()
    T_STA = auto()
    T_STS = auto()
    T_STX = auto()
    T_SUB = auto()
    T_SWI = auto()
    T_TAB = auto()
    T_TAP = auto()
    T_TBA = auto()
    T_TPA = auto()
    T_TST = auto()
    T_TSX = auto()
    T_TXS = auto()
    T_WAI = auto()
