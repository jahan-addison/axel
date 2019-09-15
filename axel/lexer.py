import re
from axel.tokens import TokenEnum, Token, Register, Mnemonic
from typing import Optional, TypeVar
from mypy_extensions import TypedDict

M = TypeVar('M', bound='Lexer')


class Lexer:

    def __init__(self, source: str) -> None:
        Scanner = TypedDict('Scanner', {
            'token': TokenEnum,
            'data': Optional[str]
        })
        self._source: str = source
        self._pointer: int = 0
        self.yylex: Scanner = {
            'token': Token.T_UNKNOWN,
            'data': None
        }
        self._last: TokenEnum = Token.T_UNKNOWN

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
        token: Optional[TokenEnum] = None
        if not term:
            raise StopIteration('Lexer Iterator out of bounds')

        self._reset()

        token = token or self._mnemonic_token(term)

        token = token or self._comma_token(term)

        token = token or self._register_token(term)

        token = token or self._equal_token(term)

        token = token or self._immediate_token(term)

        token = token or self._direct_or_extended_token(term)

        token = token or self._equal_token(term)

        token = token or self._label_token(term)

        token = token or self._displacement_token(term)

        token = token or self._lvalue_token(term)

        return token or Token.T_UNKNOWN

    def _inc(self) -> None:
        self._pointer += 1

    def _dec(self) -> None:
        self._pointer -= 1

    def _read_term(self) -> str:
        term: str = ''
        self._skip_whitespace_and_comments()
        while self.pointer and not re.match('[,\r\n\t ]', self.pointer):
            term += self.pointer
            self._inc()
        return term

    def _peek_next(self) -> str:
        term: str = ''
        self._skip_whitespace_and_comments()
        index: int = self._pointer
        size: int = len(self._source)
        while index < size and not re.match('[,\r\n\t ]', self._source[index]):
            term += self._source[index]
            index += 1

        return term

    def _reset(self) -> None:
        self.yylex = {
            'token': Token.T_UNKNOWN,
            'data': None
        }

    def _set_token(self, token: TokenEnum, term: str) -> None:
        self._last = token
        self.yylex = {
            'token': token,
            'data': term
        }

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

    def _lvalue_token(self, term: str) -> Optional[TokenEnum]:
        if self._peek_next() == '=':
            self._set_token(Token.T_LVALUE, term)
            return Token.T_LVALUE
        return None

    def _comma_token(self, term: str) -> Optional[TokenEnum]:
        if self._source[self._pointer] == ',':
            self._inc()
            self._set_token(Token.T_COMMA, term)
            return Token.T_COMMA
        return None

    def _label_token(self, term: str) -> Optional[TokenEnum]:
        if self._source[self._pointer - (len(term) + 1)] == "\n":
            if f'T_{self._peek_next()}' in Mnemonic.__members__ or \
                    term[-1:] == ':':
                self._set_token(Token.T_LABEL, term)
                return Token.T_LABEL
        return None

    def _equal_token(self, term: str) -> Optional[TokenEnum]:
        if term == '=':
            self._set_token(Token.T_EQUAL, term)
            return Token.T_EQUAL
        return None

    def _immediate_token(self, term: str) -> Optional[TokenEnum]:
        if term[:1] == '#' and term[1:2] == '$':
            if len(bytes.fromhex(term[2:])) == 1:
                self._set_token(Token.T_IMM_UINT8, term)
                return Token.T_IMM_UINT8
            elif len(bytes.fromhex(term[2:])) == 2:
                self._set_token(Token.T_IMM_UINT16, term)
                return Token.T_IMM_UINT16
        return None

    def _direct_or_extended_token(self, term: str) -> Optional[TokenEnum]:
        if term[:1] == '$':
            if len(bytes.fromhex(term[1:])) == 1:
                self._set_token(Token.T_DIR_ADDR_UINT8, term)
                return Token.T_DIR_ADDR_UINT8
            elif len(bytes.fromhex(term[1:])) == 2:
                self._set_token(Token.T_EXT_ADDR_UINT16, term)
                return Token.T_EXT_ADDR_UINT16
        return None

    def _displacement_token(self, term: str) -> Optional[TokenEnum]:
        if self._last in Mnemonic or self._last in Register:
            if f'T_{term[3:]}' not in Register.__members__ and \
                    self._peek_next() != '=' or \
                    term[-1:] == ':':
                self._set_token(Token.T_DISP_ADDR_INT8, term)
                return Token.T_DISP_ADDR_INT8
        return None

    def _mnemonic_token(self, term: str) -> Optional[TokenEnum]:
        if len(term) == 3 and \
                f'T_{term[:3]}' in Mnemonic.__members__:
            self._set_token(Mnemonic[f'T_{term[:3]}'], term[:3])
            return Mnemonic[f'T_{term[:3]}']

        if len(term) == 4 and \
                f'T_{term[:3]}' in Mnemonic.__members__ and \
                f'T_{term[3:]}' in Register.__members__:
            self._dec()
            self._set_token(Mnemonic[f'T_{term[:3]}'], term[:3])
            return Mnemonic[f'T_{term[:3]}']
        return None

    def _register_token(self, term: str) -> Optional[TokenEnum]:
        if self._source[self._pointer + 1] == 'X':
            self._inc()
            self._set_token(Register.T_X, 'X')
            return Register.T_X

        if f'T_{term}' in Register.__members__:
            self._set_token(Register[f'T_{term}'], term)
            return Register[f'T_{term}']
        return None
