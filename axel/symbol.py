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
from typing import Union, Dict, Tuple


class U_Int8:
    def __init__(self, num: int) -> None:
        self.num = num & 255

    def __add__(self, of: int) -> int:
        return (self.num + of) & 255

    def __iadd__(self, of: int) -> int:
        self.num = (self.num + of) & 255
        return self.num

    def __sub__(self, of: int) -> int:
        return (self.num - of) & 255

    def __isub__(self, of: int) -> int:
        self.num = (self.num - of) & 255
        return self.num


class U_Int16:
    def __init__(self, num: int) -> None:
        self.num = num & 65535

    def __add__(self, of: int) -> int:
        return (self.num + of) & 65535

    def __iadd__(self, of: int) -> int:
        self.num = (self.num + of) & 65535
        return self.num

    def __sub__(self, of: int) -> int:
        return (self.num - of) & 65535

    def __isub__(self, of: int) -> int:
        self.num = (self.num - of) & 65535
        return self.num


TableField = Tuple[U_Int16, str, Union[U_Int16, str]]


class Symbol_Table:
    def __init__(self) -> None:
        self.table: Dict[str, TableField] = {}

    def set(self, label:
            str, addr: U_Int16,
            type: str,
            value: Union[U_Int16, str]) -> None:
        self.table[label] = (addr, type, value)

    def get(self, label: str) -> TableField:
        return self.table[label]
