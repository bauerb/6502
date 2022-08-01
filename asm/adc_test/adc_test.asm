.debuginfo on
.listbytes unlimited

.segment "DATA"

sum1:         .res 1, $4
sum2:         .res 1, $5
erg:          .res 1, $0

.segment "CODE"
.proc main

  lda sum1
  clc
  adc sum2
  sta erg

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
