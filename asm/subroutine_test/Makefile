AS=ca65
LD=ld65

ASFLAGS=--cpu 6502 -l subroutine_test.lst -o
LDFLAGS=--config linker.ld -vm -m subroutine_test.map -o

TARGET=subroutine_test.bin
OBJ=subroutine_test.o

%.o: %.asm
	$(AS) $(ASFLAGS) $@ $<

$(TARGET): $(OBJ)
	$(LD) $(LDFLAGS) $(TARGET) $(OBJ)

clean:
	rm -rf $(OBJ) $(TARGET) *.map *.lst
