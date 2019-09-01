from typing import Any, TypeVar
from enum import Enum, unique, auto

T = TypeVar('T', bound='MnemonicEnum')

class MnemonicEnum(Enum):
  def _generate_next_value_(name: T, start: int, count: int, last_values: int) -> T: # type: ignore
    return name

@unique
class Mnemonic(MnemonicEnum):
  ABA = auto()
  ADC = auto()
  ADD = auto()
  AND = auto()
  ASL = auto()
  ASR = auto()
  BCC = auto()
  BCS = auto()
  BEQ = auto()
  BGE = auto()
  BGT = auto()
  BHI = auto()
  BIT = auto()
  BLE = auto()
  BLS = auto()
  BLT = auto()
  BMI = auto()
  BNE = auto()
  BPL = auto()
  BRA = auto()
  BSR = auto()
  BVC = auto()
  BVS = auto()
  CBA = auto()
  CLC = auto()
  CLI = auto()
  CLR = auto()
  CLV = auto()
  CMP = auto()
  COM = auto()
  CPX = auto()
  DAA = auto()
  DEC = auto()
  DES = auto()
  DEX = auto()
  EOR = auto()
  INC = auto()
  INS = auto()
  INX = auto()
  JMP = auto()
  JSR = auto()
  LDA = auto()
  LDS = auto()
  LDX = auto()
  LSR = auto()
  NEG = auto()
  NOP = auto()
  ORA = auto()
  PSH = auto()
  PUL = auto()
  ROL = auto()
  ROR = auto()
  RTI = auto()
  RTS = auto()
  SBA = auto()
  SBC = auto()
  SEC = auto()
  SEI = auto()
  SEV = auto()
  STA = auto()
  STS = auto()
  STX = auto()
  SUB = auto()
  SWI = auto()
  TAB = auto()
  TAP = auto()
  TBA = auto()
  TPA = auto()
  TST = auto()
  TSX = auto()
  TXS = auto()
  WAI = auto()