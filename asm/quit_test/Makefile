AS=ca65
LD=ld65

ASFLAGS=--cpu 6502 -l quit_test.lst -o
LDFLAGS=--config linker.ld -vm -m quit_test.map -o

TARGET=quit_test.bin
OBJ=quit_test.o

%.o: %.asm
	$(AS) $(ASFLAGS) $@ $<

$(TARGET): $(OBJ)
	$(LD) $(LDFLAGS) $(TARGET) $(OBJ)

clean:
	rm -rf $(OBJ) $(TARGET) *.map *.lst
