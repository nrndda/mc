   #include <avr/io.h>   
   #include <util/delay.h>
   #include <avr/interrupt.h>
	int sensor1,sensor2;
	
/*	SIGNAL (SIG_INTERRUPT0)
	{
	  sensor1 = 1; //PORTB = 0b00000010;
	}
	SIGNAL (SIG_INTERRUPT1)
	{
	  sensor2 = 1; //PORTB = 0b00000100;	  
	}*/

	int main(void)
	{
		DDRB = 0xff;
		PORTB = 0x00;
		DDRD = 0x00;
		PORTD = 0xff;
//		GIMSK = 0x00;
//		MCUCR = 0b00000000;	/* Прерывание от INT0 и INT1 низким напряжением */
//		GICR  = 0b11000000;
//		sei();
 		while(1)
		{	
			if ((PORTD & 0x04) == 0x04) PORTB = 0b00000001; 
			else if ((PORTD & 0x08) == 0x08) PORTB = 0b00000001;
			else PORTB = 0b00000000;
			
/*			if (sensor1) { 
			    PORTB = 0b00000001; 
			   // _delay_ms(5); 
			    sensor1 = 0;
			} else	
				if (sensor2) { 
				  PORTB = 0b00000001; 
				 // _delay_ms(5); 
				  sensor2 = 0;
				} else  PORTB = PORTB && 0xfe;*/
		}
		return 0;
   	} 

