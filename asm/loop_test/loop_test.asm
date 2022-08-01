.debuginfo on
.listbytes unlimited

.segment "DATA"

counter: .res 1, $0

.segment "CODE"
.proc main

  lda #$0
  sta counter
loop:
  lda counter
  clc
  adc #$1
  sta counter
  lda #$10
  clc
  sbc counter
  beq finish
  jmp loop

finish:
  jmp finish

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

.segment "VECTORTBL"
.word nmi
.word reset
.word irq
