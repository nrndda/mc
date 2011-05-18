INCPATH = -I/usr/avr/include -I.
OBJECTS = main lcd 
OFILES = $(OBJECTS:%=%.o)
CFILES = $(OBJECTS:%=%.c)

main.hex: main
	avr-objcopy -O ihex main main.hex
main: $(CFILES) Makefile
	avr-gcc -O -Wall $(INCPATH) -mmcu=atmega162 -o main $(CFILES)

# PROGCONF=-c stk500v2 -p m48 -P /dev/ttyACM0

# upload: main.hex
#	avrdude $(PROGCONF) -U flash:w:main.hex -F

#setfuses:
#	avrdude $(PROGCONF) -U hfuse:w:0xDF:m
#	avrdude $(PROGCONF) -U lfuse:w:0xE2:m

clean:
	rm -f main.hex main
