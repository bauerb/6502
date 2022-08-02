AS=ca65
LD=ld65

ASFLAGS=--cpu 6502 -l acia_test.lst -o
LDFLAGS=--config linker.ld -vm -m acia_test.map -o

TARGET=acia_test.bin
OBJ=acia_test.o

%.o: %.asm
	$(AS) $(ASFLAGS) $@ $<

$(TARGET): $(OBJ)
	$(LD) $(LDFLAGS) $(TARGET) $(OBJ)

clean:
	rm -rf $(OBJ) $(TARGET) *.map *.lst
