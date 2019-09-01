from enum import Enum, unique
from .opcode import Mnemonic

@unique
class Token(Enum):
  T_VARIABLE = 1,
  T_LABEL = 2,
  T_EQUAL = 3,
  T_RVALUE = 4,
  T_IMM_U8 = 5,
  T_IMM_U16 = 6,
  T_DIR_ADDR_U16 = 7,
  T_EXT_ADDR_U16 = 8,
  T_DISP_ADDR_S8 = 9
  T_MNEMONIC = 10,
  T_OPERAND = 11,
  T_PLUS = 12,
  T_INTEGER = 13,
  T_REGISTER = 14,
  T_UNKNOWN = 15


@unique
class AddressingMode(Enum):
  ACC = 50,
  IMM = 51,
  DIR = 52,
  EXT = 53,
  IDX = 54,
  INH = 55,
  REL = 56


class Lexer:
  pass



