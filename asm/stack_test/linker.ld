MEMORY {
    ZP:       start = $0000 , size = $0100, fill=yes;
    STACK:    start = $0100 , size = $0100, fill=yes;
    RAM:      start = $0200 , size = $7E00, fill=yes;
    ROM:      start = $8000 , size = $7FFA, fill=yes;
    VECTABLE: start = $FFFA , size = $0006, fill=yes;
}
SEGMENTS {
    ZP:        LOAD = RAM,      type = rw, optional=yes;
    STACK:     LOAD = RAM,      type = rw, optional=yes;
    DATA:      Load = RAM,      type = rw, optional=yes;
    CODE:      Load = ROM,      type = rw, optional=yes;
    VECTORTBL: Load = VECTABLE, type = rw, optional=yes;
}
