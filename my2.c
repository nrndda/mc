#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 8000000UL  // 8 MHz

int main(void) 
{
   DDRD = 0b00000001; 
   PORTD = 0b11111110; 
		
		while (1) {  

			if (PIND1==0) {
					PORTD = 0b11111110;
			}
			else{
					PORTD = 0b11111111;
			}
			
		} 
}