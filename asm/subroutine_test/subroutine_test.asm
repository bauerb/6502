.debuginfo on
.listbytes unlimited

.segment "DATA"

.segment "CODE"
.proc main

  lda #$05
  jsr add50

finish:
  jmp finish

.endproc

.proc add50

  clc
  adc #$50
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

.segment "VECTORTBL"
.word nmi
.word reset
.word irq
