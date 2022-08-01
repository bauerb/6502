AS=ca65
LD=ld65

ASFLAGS=--cpu 6502 -l loop_test.lst -o
LDFLAGS=--config linker.ld -vm -m loop_test.map -o

TARGET=stack_test.bin
OBJ=stack_test.o

%.o: %.asm
	$(AS) $(ASFLAGS) $@ $<

$(TARGET): $(OBJ)
	$(LD) $(LDFLAGS) $(TARGET) $(OBJ)

clean:
	rm -rf $(OBJ) $(TARGET) *.map *.lst
