AS=ca65
LD=ld65

ASFLAGS=--cpu 6502 -l adc_test.lst -o
LDFLAGS=--config linker.ld -vm -m adc_test.map -o

TARGET=adc_test.bin
OBJ=adc_test.o

%.o: %.asm
	$(AS) $(ASFLAGS) $@ $<

$(TARGET): $(OBJ)
	$(LD) $(LDFLAGS) $(TARGET) $(OBJ)

clean:
	rm -rf $(OBJ) $(TARGET) *.map *.lst
