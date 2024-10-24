.debuginfo on
.listbytes unlimited

.segment "DATA"

.segment "CODE"
.proc main

  jmp *

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
