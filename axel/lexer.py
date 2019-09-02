from typing import Optional, TypeVar, Tuple, Union
from enum import Enum, unique, auto

T = TypeVar('T', bound='TokenEnum')


class TokenEnum(Enum):
    def _generate_next_value_(  # type: ignore
            name: T,
            start: int,
            count: int,
            last_values: int) -> Tuple[T, int]:
        return (name, count + 1)


@unique
class Token(TokenEnum):
    T_LVALUE = auto(),
    T_LABEL = auto(),
    T_EQUAL = auto(),
    T_RVALUE = auto(),
    T_IMM_UINT8 = auto(),
    T_IMM_UINT16 = auto(),
    T_DIR_ADDR_UINT16 = auto(),
    T_EXT_ADDR_UINT16 = auto(),
    T_DISP_ADDR_INT8 = auto()
    T_MNEMONIC = auto(),
    T_REGISTER = auto(),
    T_UNKNOWN = auto(),
    T_EOF = auto()


@unique
class AddressingMode(TokenEnum):
    ACC = auto(),
    IMM = auto(),
    DIR = auto(),
    EXT = auto(),
    IDX = auto(),
    INH = auto(),
    REL = auto()


M = TypeVar('M', bound='Lexer')


class Lexer:

    _pointer: int
    _data: Optional[Tuple[
        str, int, Union[str, int]]]

    def __init__(self) -> None:
        pass

    def __iter__(self: M) -> M:
        return self

    def __next__(self) -> Token:
        return Token.T_UNKNOWN

    def _reset(self) -> None:
        pass

    def _peek_next(self) -> None:
        pass

    def _skip_whitespace_and_comments(self) -> None:
        pass

    def _variable_token(self) -> None:
        pass

    def _label_token(self) -> None:
        pass

    def _equal_token(self) -> None:
        pass

    def _rvalue_token(self) -> None:
        pass

    def _uint8_immediate_dt_token(self) -> None:
        pass

    def _uint16_immediate_dt_token(self) -> None:
        pass

    def _uint8_direct_dt_token(self) -> None:
        pass

    def _uint16_extended_dt_token(self) -> None:
        pass

    def _int8_displacement_dt_token(self) -> None:
        pass

    def _mnemonic_token(self) -> None:
        pass

    def _register_token(self) -> None:
        pass

    def _eof_token(self) -> None:
        pass