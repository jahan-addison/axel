# Axel
## Motorola 6800 assembler

<img src="https://upload.wikimedia.org/wikipedia/commons/5/5a/Motorola_MC6800_microprocessor.jpg" width="780" />

## Details

### Program:

* LL(1) of the 6800 assembly language
* Multipass assembler:
    * Symbol table and IR pass
    * Opcode byte translation pass
* Fully tested with robust test suite and coverage

### Device:

  * Program, data and stack memories occupy the same memory space. The total big-endian addressable memory size is 64 KB

# Development

During development, run `make start` to start and run assembler on the `healthkit` program in `etc/`.

### Tests

Run `make test`

## License

GPL-v3.0
