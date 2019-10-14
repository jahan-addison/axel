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
from io import BytesIO
from collections import deque
from typing import Deque, Union
from axel.symbol import U_Int16

program: BytesIO = BytesIO()

stack_segment: U_Int16 = U_Int16(0)
program_stack: Deque[Union[int, str]] = deque()

program_counter: U_Int16 = U_Int16(0)
