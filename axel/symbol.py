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
from typing import Union, Dict, Tuple
from ctypes import c_uint16

TableField = Tuple[c_uint16, str, Union[c_uint16, str, bytes]]


class Symbol_Table:
    def __init__(self) -> None:
        self.table: Dict[str, TableField] = {}

    def set(self,
            label: str,
            addr: c_uint16,
            type: str,
            value: Union[c_uint16, str, bytes]) -> None:

        self.table[label] = (addr, type, value)

    def get(self, label: str) -> TableField:
        return self.table[label]
