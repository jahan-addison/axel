import re
from typing import Optional, TypeVar, Tuple, Union
from enum import Enum, unique, auto

T = TypeVar('T', bound='TokenEnum')


class TokenEnum(Enum):
    def _generate_next_value_(  # type: ignore
            name: T,
            start: int,
            count: int,
            last_values: int) -> T:
        return name


@unique
class Token(TokenEnum):
    T_LVALUE = auto()
    T_LABEL = auto()
    T_EQUAL = auto()
    T_COMMA = auto()
    T_MNEMONIC = auto()
    T_REGISTER = auto()
    T_UNKNOWN = auto()
    T_IMM_UINT8 = auto()
    T_IMM_UINT16 = auto()
    T_DIR_ADDR_UINT8 = auto()
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
    T_A = auto()
    T_B = auto()
    T_X = auto()
    T_PC = auto()
    T_SP = auto()
    T_SR = auto()


@unique
class Mnemonic(TokenEnum):
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


M = TypeVar('M', bound='Lexer')


class Lexer:

    def __init__(self, source: str) -> None:
        self._pointer: int = 0
        self.yylex = {
            'token': Token.T_UNKNOWN,
            'data': None
        }
        self._last: Token = Token.T_UNKNOWN
        self._source: str = source
        self._data: Optional[
            Tuple[str, int, Union[str, int]]]

    @property
    def pointer(self) -> str:
        try:
            return self._source[self._pointer]
        except IndexError:
            return ''

    def __iter__(self: M) -> M:
        return self

    def __next__(self) -> TokenEnum:
        term = self._read_term()
        if not term:
            raise StopIteration('No more tokens!')

        if f'T_{term}' in Mnemonic.__members__:
            return Mnemonic[f'T_{term}']

        if f'T_{term}' in Register.__members__:
            return Register[f'T_{term}']

        if self._peek_next() == '=':
            return Token.T_LVALUE

        if term[:1] == '$':
            if len(bytes.fromhex(term[1:])) == 1:
                return Token.T_DIR_ADDR_UINT8
            elif len(bytes.fromhex(term[1:])) == 2:
                return Token.T_EXT_ADDR_UINT16

        if term == '=':
            return Token.T_EQUAL

        # if f'T_{self._peek_next()}' not in Mnemonic.__members__
        #     and
        return Token.T_UNKNOWN

    def _inc(self) -> None:
        self._pointer += 1

    def _reset(self) -> None:
        pass

    def _read_term(self) -> str:
        term: str = ''
        self._skip_whitespace_and_comments()
        while self.pointer and not re.match('[\r\n\t ]', self.pointer):
            term += self.pointer
            self._inc()
        return term

    def _peek_next(self) -> str:
        term: str = ''
        self._skip_whitespace_and_comments()
        index: int = self._pointer
        size: int = len(self._source)
        while index < size and not re.match('[\r\n\t ]', self._source[index]):
            term += self._source[index]
            index += 1

        return term

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
            if not self.pointer:
                skip = False
                break
            self._inc()
            if self.pointer == '\n' \
                    or self.pointer == '\r':
                is_newline = True
            if is_newline:
                if self.pointer != '\n' \
                        and self.pointer != '\r':
                    skip = False

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
