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
import axel.tokens as Tokens
from collections import deque
from typing import Union, List, overload, Deque, Tuple
from axel.lexer import Lexer, yylex_t
from axel.symbol import Symbol_Table, U_Int16

Token_T = Union[Tokens.Token, Tokens.Mnemonic, Tokens.Register]


class AssemblerParserError(Exception):
    pass


class Parser:
    def __init__(self, source: str,
                 symbols: Symbol_Table = Symbol_Table()) -> None:
        self._line = 1
        self.lexer: Lexer = Lexer(source)
        self.symbols: Symbol_Table = symbols

    def error(self, expected: str, found: Tokens.TokenEnum) -> None:
        location = self.lexer.last_addr
        source = self.lexer._source[location:location + 12].replace('\n', ' ')
        raise AssemblerParserError(
            f'Parser failed near "{source}", '
            f'expected one of {expected}, '
            f'but found "{found.name}" '
            f'on line {self._line}.')

    def parse_immediate_value(self, value: str) -> bytes:
        """TODO: Decimal, binary, single character. """
        if value[:1] == '#' and value[1:2] == '$':
            return bytes.fromhex(value[2:])
        else:
            return bytes.fromhex(value[1:])

    @overload
    def take(self, test: List[Token_T]) -> None: ...

    @overload
    def take(self, test: Token_T) -> None: ...

    def take(self, test: Union[Token_T, List[Token_T]]) -> None:
        lexer = self.lexer
        next_token = next(lexer)
        if isinstance(test, list):
            if next_token not in test:
                options = list(map(lambda x: x.name, test))
                lexer.retract()
                self.error(', '.join(options), next_token)
        else:
            if next_token is not test:
                lexer.retract()
                self.error(test.name, next_token)

    def line(self) -> Union[
            Tuple[Tokens.TokenEnum, Deque[yylex_t]],
            bool]:
        lexer = self.lexer
        test = [
            Tokens.Token.T_LABEL.name,
            Tokens.Token.T_VARIABLE.name,
            Tokens.Token.T_MNEMONIC.name]
        try:
            next(lexer)
            current = lexer.yylex['token']
            while current == Tokens.Token.T_EOL:
                # skip empty line sequences
                self._line += 1
                next(lexer)
                current = lexer.yylex['token']
            if current == Tokens.Token.T_LABEL:
                self.take(list(Tokens.Mnemonic))
                line = self.instruction(lexer.yylex)
                self.take(Tokens.Token.T_EOL)
                self._line += 1
                return line
            elif current == Tokens.Token.T_VARIABLE:
                self.variable(lexer.yylex)
                self.take(Tokens.Token.T_EOL)
                self._line += 1
                return True
            elif isinstance(current, Tokens.Mnemonic):
                line = self.instruction(lexer.yylex)
                self.take(Tokens.Token.T_EOL)
                self._line += 1
                return line
        except StopIteration:
            return False  # Done.

        # Should never get here.
        self.error(', '.join(test), lexer.yylex['token'])
        return False

    def variable(self, label: yylex_t) -> None:
        name = label['data']
        addr = self.lexer.last_addr
        self.take(Tokens.Token.T_EQUAL)
        self.take([Tokens.Token.T_DIR_ADDR_UINT8,
                   Tokens.Token.T_EXT_ADDR_UINT16])
        if isinstance(name, str) and self.lexer.yylex['data'] is not None:
            # re-set in the symbol table in byte format
            symbol = self.symbols.get(name)
            if symbol is not None and isinstance(symbol[2], str):
                self.symbols.set(
                    name,
                    U_Int16(addr),
                    'variable',
                    self.parse_immediate_value(symbol[2]))
            else:
                raise AssemblerParserError(
                    f'Parser failed on variable "{name}"')

    def operands(self) -> Deque[yylex_t]:
        stack: Deque[yylex_t] = deque()
        datatypes = [
            Tokens.Token.T_IMM_UINT8,
            Tokens.Token.T_IMM_UINT16,
            Tokens.Token.T_DIR_ADDR_UINT8,
            Tokens.Token.T_EXT_ADDR_UINT16,
            Tokens.Token.T_DISP_ADDR_INT8,
        ]
        while True:
            try:
                self.take([
                    *list(Tokens.Register),
                    Tokens.Token.T_COMMA,
                    *datatypes])
                stack.appendleft(self.lexer.yylex)
            except AssemblerParserError:
                self.lexer.retract()
                break
            except StopIteration:
                break
        return stack

    def instruction(
            self,
            instruction: yylex_t) -> Tuple[Tokens.TokenEnum, Deque[yylex_t]]:
        return (instruction['token'], self.operands())
