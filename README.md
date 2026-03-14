<h5 align="center">
  Motorola MC6800 Assembler 💻
</h5>

<table align="center">
<tr>
<td>

```text
                 /\
   Lionheart  ( ;`~v/~~~ ;._
              ,/'"/^) ' < o\  '".~'\\\--,
            ,/",/W  u '`. ~  >,._..,   )'
          ,/'  w  ,U^v  ;//^)/')/^\;~)'
        ,/"'/   W` ^v  W |;         )/'
      ;''  |  v' v`" W }  \\
    "    .'\    v  `v/^W,) '\)\.)\/)
              `\   ,/,)'   ''')/^"-;'
                  \
                ".
```
</td>
</tr>
</table>

<!-- <h1 align="center">lionheart </h1> -->

## Overview

The MC6800 ("sixty-eight hundred") is an 8-bit microprocessor designed and first manufactured by Motorola in 1974. The MC6800 microprocessor was part of the M6800 Microcomputer System that also included serial and parallel interface ICs, RAM, ROM and other support chips.


<table border="0">
  <tr>
    <td width="220">
      <img src="/docs/MC6800.jpeg" width="200">
    </td>
    <td>
      The MC6800 has a 16-bit address bus that can directly access 64 kB of memory and an 8-bit bi-directional data bus. It has 72 instructions with seven addressing modes for a total of 197 opcodes. The original MC6800 could have a clock frequency of up to 1 MHz. Later versions had a maximum clock frequency of 2 MHz.
    </td>
  </tr>
</table>

<!-- <p align="center">
   <img width="700px" src="/docs/MC6800.jpeg">
</p> -->

## Assembler

### Usage:

```bash
./lionheart <input_file> [output_file]
```

### Features

* Excellent error handling, easy-to-extend parsing with [cpp-peglib](https://github.com/yhirose/cpp-peglib)

#### Directives

* `ORG` mnemonic to set program counter, set to address `$F000` if omitted
* `FDB` mnemonic to set interrupt and reset vectors

**Note**: [hexed.it](https://hexed.it) is a great online hex editor to disassemble bytecode!

### Example:

```asm
; Start of ROM
ORG $F000

RESET:
    ; Setup stack register (required)
    LDS #$00FF
    ; Clear accumulators (optional)
    CLR A
    CLR B
    ; Clear the index register (optional)
    LDX #$0000
    ; Enable interrupts
    CLI

MAIN:
    ; My Program
    LDA A #$42      ; Load the meaning of life

    ; Reset vector
    ORG $FFFE       ; The exact end of memory
    FDB RESET
```

#### Error messages:

```asm
ZUES = $FE3A

ORG $F010
START:
  JSR DONE
  LDA A #$01
  TAB
  TST ; <-- bad
  BRA START

DONE:
  FDB START
```

<img src="/docs/error-example.png" width="600px">

---

## Build

```bash
cmake -Bbuild -DCMAKE_BUILD_TYPE=Debug -DUSE_SANITIZER="Address;Undefined" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DIWYU=ON
cmake --build build
```

#### Windows

```bash
cmake -Bbuild -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build
```

Or with `ninja`:

```bash
cmake -Bbuild -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build
```

---

Run the test suite:

```bash
./build/test_suite
```

## Licensing

This project is dual-licensed under the **Apache License, Version 2.0** and the **GNU General Public License, Version 3.0 (or later)**.

You are free to choose the license that best fits your specific use case. For the full text of each license, please see [LICENSE.Apache-v2](LICENSE.Apache-v2) and [LICENSE-GPL-v3](LICENSE.GPL-v3).