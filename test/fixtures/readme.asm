; Variables
DATA    = $F0

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
		LDA A #$01
		BRA OUT
SAME:
		LDA B DATA
		ADD B #$10
		STA B DATA
		ASL A
OUT:
		LDX #$2F00
WAIT:
		DEX
		BNE WAIT
		TAB
		TST B
		BNE SAME
		LDA A #$01
		LDX $F0
		CPX #$C10F
		BNE OUT
		BRA MAIN

    ; Reset vector
    ORG $FFFE       ; The exact end of memory
    FDB RESET
