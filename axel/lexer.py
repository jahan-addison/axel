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
import re
from axel.tokens import TokenEnum, Lexeme as Token, Register, Mnemonic
from typing import Optional, TypeVar
from mypy_extensions import TypedDict

M = TypeVar('M', bound='Lexer')


class Yylex(TypedDict, total=False):
    token: TokenEnum
    data: Optional[str]


class Lexer:
    """Lexer Iterator for the 6800 assembly language.

    LL(1) tokenization and scanner of a source string into a stream,
    sets last token and scanner data during each __next__ call.

    """
    def __init__(self, source: str) -> None:
        self._source: str = source
        self._pointer: int = 0
        self.yylex: Yylex = {
            'token': Token.T_UNKNOWN,
            'data': None
        }
        self._at = self._pointer
        self._last: TokenEnum = Token.T_UNKNOWN

    @property
    def pointer(self) -> str:
        """Pointer of current location in scanner. """
        try:
            return self._source[self._pointer]
        except IndexError:
            return ''

    @property
    def last_addr(self) -> int:
        """The pointer index of source before the last iteration. """
        return self._at

    def __iter__(self: M) -> M:
        """Iterator instance. """
        return self

    def __next__(self) -> TokenEnum:
        """Scan and retrieve next token on each iteration. """
        self._at = self._pointer
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

        token = token or self._variable_token(term)

        return token or Token.T_UNKNOWN

    def retract(self) -> None:
        """Set the pointer to before the last token.

        This is useful during parsing to deal with erroneous acceptance."""
        self._pointer = self._at

    def _inc(self) -> None:
        """Increment the pointer, similar to pointer arithmetic."""
        self._pointer += 1

    def _dec(self) -> None:
        """Decrement the pointer, similar to pointer arithmetic."""
        self._pointer -= 1

    def _read_term(self) -> str:
        """Read next term.

        Read the next term from the source string, skipping all
        whitespace and comments.
        """
        term: str = ''
        self._skip_whitespace_and_comments()
        while self.pointer and not re.match('[,\r\n\t ]', self.pointer):
            term += self.pointer
            self._inc()
        return term

    def _peek_next(self) -> str:
        """Peek one term.

        Peek ahead one term to infer lexical analysis.
        """
        term: str = ''
        self._skip_whitespace_and_comments()
        index: int = self._pointer
        size: int = len(self._source)
        while index < size and not re.match('[,\r\n\t ]', self._source[index]):
            term += self._source[index]
            index += 1

        return term

    def _reset(self) -> None:
        """Reset scanner data. """
        self.yylex = {
            'token': Token.T_UNKNOWN,
            'data': None
        }

    def _set_token(self, token: TokenEnum, term: str) -> None:
        """Set last token and scanner data. """
        self._last = token
        self.yylex = {
            'token': token,
            'data': term
        }

    def _skip_whitespace_and_comments(self) -> None:
        """Skip whitespace.

        Skips all whitespace and comments recursively until the beginning
        of the next term.
        """
        if re.match('[\r\n\t ]', self.pointer):
            self._inc()
            self._skip_whitespace_and_comments()
        if self.pointer == ';':
            self._skip_to_next_line()
            self._skip_whitespace_and_comments()

    def _skip_to_next_line(self) -> None:
        """ Skips until sequences of EOL. """
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

    def _variable_token(self, term: str) -> Optional[TokenEnum]:
        """Tokenize variables. (i.e. *addressable* operands) """
        if self._peek_next() == '=':
            self._set_token(Token.T_VARIABLE, term)
            return Token.T_VARIABLE
        return None

    def _comma_token(self, term: str) -> Optional[TokenEnum]:
        """Commas.

        Tokenize operands between general registers and the index register (X).
        """
        if self.pointer == ',':
            self._inc()
            self._set_token(Token.T_COMMA, term)
            return Token.T_COMMA
        return None

    def _label_token(self, term: str) -> Optional[TokenEnum]:
        """Labels.

        Tokenize labels at the beginning of lines, and
        optionally suffixed with ":".
        """
        peek_back = self._pointer - (len(term) + 1)
        previous_line = self._source[peek_back]
        if previous_line == "\n" or peek_back <= 0:
            if f'T_{self._peek_next()}' in Mnemonic.__members__ or \
                    term[-1:] == ':':
                self._set_token(Token.T_LABEL, term)
                return Token.T_LABEL
        return None

    def _equal_token(self, term: str) -> Optional[TokenEnum]:
        """Equal assignment.

        Tokenize equal assignments between variables and
        addressable operands."""
        if term == '=':
            self._set_token(Token.T_EQUAL, term)
            return Token.T_EQUAL
        return None

    def _immediate_token(self, term: str) -> Optional[TokenEnum]:
        """Tokenize immediate 1 byte or 2 byte data. """
        if term[:1] == '#' and term[1:2] == '$':
            if len(bytes.fromhex(term[2:])) == 1:
                self._set_token(Token.T_IMM_UINT8, term)
                return Token.T_IMM_UINT8
            elif len(bytes.fromhex(term[2:])) == 2:
                self._set_token(Token.T_IMM_UINT16, term)
                return Token.T_IMM_UINT16
        return None

    def _direct_or_extended_token(self, term: str) -> Optional[TokenEnum]:
        """Tokenize direct or extended 1 byte or 2 byte addressable memory.

        TODO: Decimal, binary, single character. """
        if term[:1] == '$':
            if len(bytes.fromhex(term[1:])) == 1:
                self._set_token(Token.T_DIR_ADDR_UINT8, term)
                return Token.T_DIR_ADDR_UINT8
            elif len(bytes.fromhex(term[1:])) == 2:
                self._set_token(Token.T_EXT_ADDR_UINT16, term)
                return Token.T_EXT_ADDR_UINT16
        return None

    def _displacement_token(self, term: str) -> Optional[TokenEnum]:
        """Tokenize displacement (i.e. branching) 1 byte memory addresses. """
        if self._last in Mnemonic or self._last in Register:
            if f'T_{term[3:]}' not in Register.__members__ and \
                    self._peek_next() != '=':
                self._set_token(Token.T_DISP_ADDR_INT8, term)
                return Token.T_DISP_ADDR_INT8
        return None

    def _mnemonic_token(self, term: str) -> Optional[TokenEnum]:
        """ Mnemonics.

        Tokenize Mnemonics in the 6800 ISA. Note that some mnemonics
        like the `LDA' may contiguously use its register operand `LDAA'
        in immediate addressing mode.
        Examples:
            LDAA #$01
            TAB
            TST B
        """
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
        """ Registers.

        Tokenize register operands, including the special-case index
        register `X'.
        """
        try:
            if self._source[self._pointer + 1] == 'X':
                self._inc()
                self._set_token(Register.T_X, 'X')
                return Register.T_X
        except IndexError:
            return None

        if f'T_{term}' in Register.__members__:
            self._set_token(Register[f'T_{term}'], term)
            return Register[f'T_{term}']
        return None
