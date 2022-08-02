.debuginfo on
.listbytes unlimited

ACIA := $8000
ACIAData := ACIA+0
ACIAStatus := ACIA+1
ACIACommand := ACIA+2
ACIAControl := ACIA+3

CR=13
LF=10

.segment "ZEROPAGE"

STRNG1:
  .res 2

.segment "DATA"

.segment "KERNAL"

.proc main

  lda #<Message
  ldy #>Message
  jsr printString

finish:
  brk
.endproc

.proc printString

  sta STRNG1
  sty STRNG1+1
  ldy #$FF
loop:
  iny
  lda (STRNG1),y
  beq finish
  sta ACIAData
  jmp loop

finish:
  rts
.endproc

.proc reset
  jmp main
.endproc

.proc nmi
  rti
.endproc

.proc irq
  rti
.endproc

Message:
  .byte   "Das ist ein Test"
  .byte   CR,LF,0

.segment "VECTORTBL"
.word nmi
.word reset
.word irq
