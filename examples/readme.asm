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