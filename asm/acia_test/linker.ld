MEMORY {
    ZEROPAGE: start = $0000 , size = $0100, fill=yes;
    STACK:    start = $0100 , size = $0100, fill=yes;
    RAM:      start = $0200 , size = $7E00, fill=yes;
    BASIC:    start = $C000 , size = $2000, fill=yes, file="basic.bin";
    KERNAL:   start = $E000 , size = $1FFA, fill=yes, file="kernel.bin";
    VECTABLE: start = $FFFA , size = $0006, fill=yes, file="kernel.bin";
}
SEGMENTS {
    ZEROPAGE:  LOAD = ZEROPAGE, type = rw, optional=yes;
    STACK:     LOAD = STACK,    type = rw, optional=yes;
    DATA:      Load = RAM,      type = rw, optional=yes;
    BASIC:     Load = BASIC,    type = rw, optional=yes;
    KERNAL:    Load = KERNAL,   type = rw, optional=yes;
    VECTORTBL: Load = VECTABLE, type = rw, optional=yes;
}
