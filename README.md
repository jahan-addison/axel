# axel [![Build Status](https://travis-ci.org/jahan-addison/axel.svg?branch=master)](https://travis-ci.org/jahan-addison/axel)

### Motorola 6800 Assembler

<img src="https://upload.wikimedia.org/wikipedia/commons/5/5a/Motorola_MC6800_microprocessor.jpg" width="780" />


> The 6800 ("sixty-eight hundred") is an 8-bit microprocessor designed and first manufactured by Motorola in 1974. The MC6800 microprocessor was part of the M6800 Microcomputer System that also included serial and parallel interface ICs, RAM, ROM and other support chips.


The 6800 has a 16-bit address bus that can directly access 64 kB of memory and an 8-bit bi-directional data bus. It has 72 instructions with seven addressing modes for a total of 197 opcodes. The original MC6800 could have a clock frequency of up to 1 MHz. Later versions had a maximum clock frequency of 2 MHz.


# Design

* Fast, LL(1) of the 6800 assembly language.
* Strongly typed, with [mypy --strict](http://mypy-lang.org/)
* Verbose errors and adept parser error recovery.
* Multipass assembler:
    * Symbol table and IR pass.
    * Opcode byte translation pass.


Fully tested with robust test suite. (90% coverage)


# Development

During development, run `make start` to start and run assembler on the `healthkit` program in `etc/`.

## Tests

Run `make test`

## License

GPL-v3.0
