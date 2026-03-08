<h1 align="center">axel 💻</h1>

<h5 align="center">
  Motorola MC6800 Assembler
</h5>
<p align="center">
   <img width="700px" src="/docs/MC6800.jpeg">
</p>

## Overview

The MC6800 ("sixty-eight hundred") is an 8-bit microprocessor designed and first manufactured by Motorola in 1974. The MC6800 microprocessor was part of the M6800 Microcomputer System that also included serial and parallel interface ICs, RAM, ROM and other support chips.

The MC6800 has a 16-bit address bus that can directly access 64 kB of memory and an 8-bit bi-directional data bus. It has 72 instructions with seven addressing modes for a total of 197 opcodes. The original MC6800 could have a clock frequency of up to 1 MHz. Later versions had a maximum clock frequency of 2 MHz.

## Assembler Design

* Extendable, easy to follow PEG grammar with [cpp-peglib](https://github.com/yhirose/cpp-peglib)
* Multi-pass:
  * Symbol table and IR pass
  * Opcode byte translation pass

#### wontdo

* DATA section

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