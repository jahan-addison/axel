import re
from typing import Optional, TypeVar, Tuple, Union
from enum import Enum, unique, auto

T = TypeVar('T', bound='TokenEnum')


class TokenEnum(Enum):
    def _generate_next_value_(  # type: ignore
            name: T,
            start: int,
            count: int,
            last_values: int) -> Tuple[T, int]:
        return (name, start + 1)


@unique
class Token(TokenEnum):
    T_LVALUE = auto()
    T_LABEL = auto()
    T_EQUAL = auto()
    T_RVALUE = auto()
    T_MNEMONIC = auto()
    T_REGISTER = auto()
    T_UNKNOWN = auto()
    T_IMM_UINT8 = auto()
    T_IMM_UINT16 = auto()
    T_DIR_ADDR_UINT16 = auto()
    T_EXT_ADDR_UINT16 = auto()
    T_DISP_ADDR_INT8 = auto()
    T_EOL = auto()
    T_EOF = auto()


@unique
class AddressingMode(TokenEnum):
    ACC = auto()
    IMM = auto()
    DIR = auto()
    EXT = auto()
    IDX = auto()
    INH = auto()
    REL = auto()


@unique
class Register(TokenEnum):
    A = auto()
    B = auto()
    X = auto()
    PC = auto()
    SP = auto()
    SR = auto()


@unique
class Mnemonic(TokenEnum):
    ABA = auto()
    ADC = auto()
    ADD = auto()
    AND = auto()
    ASL = auto()
    ASR = auto()
    BCC = auto()
    BCS = auto()
    BEQ = auto()
    BGE = auto()
    BGT = auto()
    BHI = auto()
    BIT = auto()
    BLE = auto()
    BLS = auto()
    BLT = auto()
    BMI = auto()
    BNE = auto()
    BPL = auto()
    BRA = auto()
    BSR = auto()
    BVC = auto()
    BVS = auto()
    CBA = auto()
    CLC = auto()
    CLI = auto()
    CLR = auto()
    CLV = auto()
    CMP = auto()
    COM = auto()
    CPX = auto()
    DAA = auto()
    DEC = auto()
    DES = auto()
    DEX = auto()
    EOR = auto()
    INC = auto()
    INS = auto()
    INX = auto()
    JMP = auto()
    JSR = auto()
    LDA = auto()
    LDS = auto()
    LDX = auto()
    LSR = auto()
    NEG = auto()
    NOP = auto()
    ORA = auto()
    PSH = auto()
    PUL = auto()
    ROL = auto()
    ROR = auto()
    RTI = auto()
    RTS = auto()
    SBA = auto()
    SBC = auto()
    SEC = auto()
    SEI = auto()
    SEV = auto()
    STA = auto()
    STS = auto()
    STX = auto()
    SUB = auto()
    SWI = auto()
    TAB = auto()
    TAP = auto()
    TBA = auto()
    TPA = auto()
    TST = auto()
    TSX = auto()
    TXS = auto()
    WAI = auto()


M = TypeVar('M', bound='Lexer')


class Lexer:

    def __init__(self, source: str) -> None:
        self._pointer: int = 0
        self._source: str = source
        self._data: Optional[
            Tuple[str, int, Union[str, int]]]

    @property
    def pointer(self) -> str:
        return self._source[self._pointer]

    def __iter__(self: M) -> M:
        return self

    def __next__(self) -> Token:
        return Token.T_UNKNOWN

    def _inc(self) -> None:
        self._pointer += 1

    def _reset(self) -> None:
        pass

    def _peek_next(self) -> None:
        pass

    def _skip_whitespace_and_comments(self) -> None:
        if re.match('[\r\n\t ]', self.pointer):
            self._inc()
            self._skip_whitespace_and_comments()
        if self.pointer == ';':
            self._skip_to_next_line()
            self._skip_whitespace_and_comments()


    def _skip_to_next_line(self) -> None:
        skip = True
        is_newline = False
        while skip:
            if self.pointer == '\n' \
                    or self.pointer == '\r':
                is_newline = True
            elif is_newline:
                if self.pointer != '\n' \
                        and self.pointer != '\r':
                    skip = False
            self._inc()

    def _variable_token(self) -> None:
        pass

    def _label_token(self) -> None:
        pass

    def _equal_token(self) -> None:
        pass

    def _rvalue_token(self) -> None:
        pass

    def _uint8_immediate_token(self) -> None:
        pass

    def _uint16_immediate_token(self) -> None:
        pass

    def _uint8_direct_token(self) -> None:
        pass

    def _uint16_extended_token(self) -> None:
        pass

    def _int8_displacement_token(self) -> None:
        pass

    def _mnemonic_token(self) -> None:
        pass

    def _register_token(self) -> None:
        pass

    def _eol_token(self) -> None:
        pass

    def _eof_token(self) -> None:
        pass
