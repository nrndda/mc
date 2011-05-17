   #include <avr/io.h>
   #include <util/delay.h>

int i;   
   int main(void)
   {
	DDRD = 7;
	DDRC = 0x80;
        while (1) {
			for (i=1;i<10;i++) {PORTC = 0x00;PORTD = 7;_delay_ms(100); PORTC = 0x80;PORTD = 7;}
			for (i=1;i<10;i++) {PORTC = 0x80;PORTD = 6;_delay_ms(100); PORTC = 0x80;PORTD = 7;}
			for (i=1;i<10;i++) {PORTC = 0x80;PORTD = 5;_delay_ms(100); PORTC = 0x80;PORTD = 7;}
			for (i=1;i<10;i++) {PORTC = 0x80;PORTD = 3;_delay_ms(100); PORTC = 0x80;PORTD = 7;}
			_delay_ms(100);
		}
   }

