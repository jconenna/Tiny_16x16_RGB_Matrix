DEVICE      = atmega2560
CLOCK       = 16000000UL
PROGRAMMER  = -c usbtiny 
OBJECTS     = 16x16_rgb.o
FUSES       = -U lfuse:w:0xc2:m -U hfuse:w:0x99:m -U efuse:w:0xff:m 
AVRDUDE     = avrdude $(PROGRAMMER) -p $(DEVICE)
COMPILE     = avr-gcc -Wall -O3 -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) -std=c99

all:	16x16_rgb.hex

.c.o:
	$(COMPILE) -c $< -o $@ 

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(COMPILE) -S $< -o $@

flash:	all
	$(AVRDUDE) -U flash:w:16x16_rgb.hex:i

fuse:
	$(AVRDUDE) $(FUSES)

make: flash fuse

load: all
	bootloadHID 16x16_rgb.hex

clean:
	rm -f 16x16_rgb.hex 16x16_rgb.elf $(OBJECTS)

16x16_rgb.elf: $(OBJECTS)
	$(COMPILE) -o 16x16_rgb.elf $(OBJECTS)

16x16_rgb.hex: 16x16_rgb.elf
	rm -f 16x16_rgb.hex
	avr-objcopy -j .text -j .data -O ihex 16x16_rgb.elf 16x16_rgb.hex
	avr-size --format=avr --mcu=$(DEVICE) 16x16_rgb.elf


