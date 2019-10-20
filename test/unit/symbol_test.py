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
from ctypes import c_uint16
from axel.symbol import Symbol_Table

def test_symbol_table_set() -> None:
    test = Symbol_Table()
    test.set('test', c_uint16(255), 'variable', 'testing')
    assert test.table['test'][0].value == 255
    assert test.table['test'][1] == 'variable'
    assert test.table['test'][2] == 'testing'


def test_symbol_table_get() -> None:
    test = Symbol_Table()
    test.table['test'] = ('test', c_uint16(255), 'variable', 'testing')
    assert test.get('test')[0] == 'test'
    assert test.get('test')[1].value == 255
    assert test.get('test')[2] == 'variable'
    assert test.get('test')[3] == 'testing'
