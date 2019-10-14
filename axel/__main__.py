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
# from axel.lexer import Lexer
from axel.parser import Parser

with open('./etc/healthkit.asm') as f:
    # Token stream:
    # test = Lexer(f.read())
    # for token in test:
    #     print(f'"{token}"')
    test = Parser(f.read())
    line = test.line()
    print('Instructions:\n')
    while line:
        if not isinstance(line, bool):
            print(line)
        line = test.line()
    print('\nSymbols:\n',
          test.symbols.table)
