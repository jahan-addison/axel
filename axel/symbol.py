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
from typing import Union, Dict, Tuple, TypeVar

X = TypeVar('X', bound='U_Int8')
Y = TypeVar('Y', bound='Int8')
M = TypeVar('M', bound='U_Int16')


class U_Int8:
    """A class datatype that behaves like a unsigned 8bit integer.
    Keeps raw numerical details for overflow and carry data.

    Provides easy-to-use operators on normal numerical types
    that lifts the result into the unsigned type.
    """
    def __init__(self, num: int) -> None:
        self.raw = num
        self.num = num & 255

    def __repr__(self) -> str:
        return str(self.num)

    def __add__(self, of: int) -> int:
        self.raw += of
        return (self.num + of) & 255

    def __iadd__(self: X, of: int) -> X:
        self.raw += of
        self.num = (self.num + of) & 255
        return self

    def __sub__(self, of: int) -> int:
        self.raw -= of
        return (self.num - of) & 255

    def __isub__(self: X, of: int) -> X:
        self.raw -= of
        self.num = (self.num - of) & 255
        return self


class Int8:
    """A class datatype that behaves like a signed 8bit integer.

    Provides easy-to-use operators on normal numerical types
    that lifts the result into the signed type with 2's complement.
    """
    def __init__(self, num: int) -> None:
        self.num = self._to_int8(num)

    def _to_int8(self, num: int) -> int:
        mask7 = 128
        mask2s = 127
        if (mask7 & num == 128):
            num = -((~int(num) + 1) & mask2s)  # 2's complement
        return num

    def __repr__(self) -> str:
        return str(self.num)

    def __add__(self, of: int) -> int:
        return self._to_int8(self.num + of)

    def __iadd__(self: Y, of: int) -> Y:
        self.num = self._to_int8(self.num + of)
        return self

    def __sub__(self, of: int) -> int:
        return self._to_int8(self.num - of)

    def __isub__(self: Y, of: int) -> Y:
        self.num = self._to_int8(self.num - of)
        return self


class U_Int16:
    """A class datatype that behaves like a unsigned 18bit integer.

    Provides easy-to-use operators on normal numerical types
    that lifts the result into the unsigned type.
    """
    def __init__(self, num: int) -> None:
        self.num = num & 65535

    def __repr__(self) -> str:
        return str(self.num)

    def __add__(self, of: int) -> int:
        return (self.num + of) & 65535

    def __iadd__(self: M, of: int) -> M:
        self.num = (self.num + of) & 65535
        return self

    def __sub__(self, of: int) -> int:
        return (self.num - of) & 65535

    def __isub__(self: M, of: int) -> M:
        self.num = (self.num - of) & 65535
        return self


TableField_T = Tuple[U_Int16, str, Union[U_Int16, str, bytes]]


class Symbol_Table:
    """Assembler Symbol table.

    Symbol table for 6800 program labels or variables
    and their location in memory, type, and data.
    """
    def __init__(self) -> None:
        self.table: Dict[str, TableField_T] = {}

    def set(self,
            label: str,
            addr: U_Int16,
            type: str,
            value: Union[U_Int16, str, bytes]) -> None:
        """ Set the table entry for a label or variable. """
        self.table[label] = (addr, type, value)

    def get(self, label: str) -> TableField_T:
        """ Get a table entry for a label or variable. """
        return self.table[label]
