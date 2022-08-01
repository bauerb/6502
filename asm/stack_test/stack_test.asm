.debuginfo on
.listbytes unlimited

.segment "DATA"

.segment "CODE"
.proc main

  lda #$55
  pha
  lda #$00
  pla

  php
  plp

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
