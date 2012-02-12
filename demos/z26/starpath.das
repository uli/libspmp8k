# DASM source for our Starpath ROM.

                processor 6502
VSYNC           equ     $00
VBLANK          equ     $01
WSYNC           equ     $02
COLUPF          equ     $08
COLUBK          equ     $09
CTRLPF          equ     $0a
PF0             equ     $0d
PF1             equ     $0e
PF2             equ     $0f
RESP0           equ     $10
RESP1           equ     $11
AUDC0           equ     $15
AUDF0           equ     $17
AUDV0           equ     $19
AUDV1           equ     $1a
GRP0            equ     $1b
GRP1            equ     $1c
ENAM0           equ     $1d
ENAM1           equ     $1e
ENABL           equ     $1f
HMP0            equ     $20
HMP1            equ     $21
HMOVE           equ     $2a

                ORG     $F800
                lda     $fa
                sta     $81
                jmp     Advance

                ORG     $F80A
                lda     #$00
                sta     $81

Advance:
                lda     #$00
                sta     GRP0
                sta     GRP1
                sta     ENAM0
                sta     ENAM1
                sta     ENABL
                sta     AUDV0
                sta     AUDV1
                sta     COLUPF
                sta     VBLANK

                lda     #$10
                sta     HMP1
                sta     WSYNC
                ldx     #$07
                dex
Pos:            dex 
                bne     Pos
                lda     #$00 
                sta     HMP0
                sta     RESP0
                sta     RESP1
                sta     WSYNC
                sta     HMOVE

                lda     #%00000101
                sta     CTRLPF
                lda     #$ff
                sta     PF0
                sta     PF1
                sta     PF2
;                sta     $83
                sta     $84
                sta     $85
                lda     #$f0
                sta     $83
                lda     #$74
                sta     COLUBK
                lda     #$0c
                sta     AUDC0
                lda     #$1f
                sta     AUDF0
                sta     $82
                lda     #$07
                sta     AUDV0
A1:
                ldx     #$08
                ldy     #$00
A2:
                sta     WSYNC
                dey
                bne     A2
                sta     WSYNC
                sta     WSYNC
                lda     #$02
                sta     WSYNC
                sta     VSYNC
                sta     WSYNC
                sta     WSYNC
                sta     WSYNC
                lda     #$00
                sta     VSYNC
                dex
                bpl     A2
                asl     $83
                ror     $84
                rol     $85
                lda     $83
                sta     PF0
                lda     $84
                sta     PF1
                lda     $85
                sta     PF2
                ldx     $82
                dex
                stx     $82
                stx     AUDF0
                cpx     #$0a
                bne     A1

                lda     #%00000010
                sta     VBLANK

                lda     $81
                ldx     #$1c
                ldy     #$00
                sty     AUDV0
                sty     COLUBK
Clear:
                sty     $81,x
                dex
                bpl     Clear
                ldx     #$ff

                and     #$ff
                bne     End
                jmp     $fff1
End:            jmp     $fff0
