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
import pytest
from axel.symbol import Symbol_Table, U_Int16, U_Int8, Int8


def test_uint8() -> None:
    uint8 = U_Int8(0)
    uint8_2 = U_Int8(255)
    assert uint8 + 5 == 5
    assert uint8 - 5 == 255 - 5 + 1
    assert uint8_2 + 1 == 0
    assert uint8_2 - 5 == 255 - 5
    uint8 += 1
    assert uint8.num == 1
    uint8 -= 2
    assert uint8.num == 255

def test_int8() -> None:
    int8 = Int8(-128)
    int8_2 = Int8(127)
    assert int8 + 5 == 5
    assert int8 - 5 == -5
    assert int8_2 + 1 == 0
    assert int8_2 - 5 == 127 - 5
    int8 += 1
    assert int8.num == 1
    int8 -= 2
    assert int8.num == -1

def test_uint16() -> None:
    uint16 = U_Int16(0)
    uint16_2 = U_Int16(65535)
    assert uint16 + 5 == 5
    assert uint16 - 5 == 65535 - 5 + 1
    assert uint16_2 + 1 == 0
    assert uint16_2 - 5 == 65535 - 5
    uint16 += 1
    assert uint16.num == 1

def test_symbol_table_set() -> None:
    test = Symbol_Table()
    test.set('test', U_Int16(255), 'variable', 'testing')
    assert test.table['test'][0].num == 255
    assert test.table['test'][1] == 'variable'
    assert test.table['test'][2] == 'testing'


def test_symbol_table_get() -> None:
    test = Symbol_Table()
    test.table['test'] = ('test', U_Int16(255), 'variable', 'testing')
    assert test.get('test')[0] == 'test'
    assert test.get('test')[1].num == 255
    assert test.get('test')[2] == 'variable'
    assert test.get('test')[3] == 'testing'
