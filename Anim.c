   #include <avr/io.h>
   #include <util/delay.h>
   #define F_CPU 8000000UL  // 8 MHz
 
typedef struct
{
	int pb, pd;
} port;
   
   int main(void)              // начало основой программы
   {
   
   DDRD = 0xff;            /* все выводы порта D сконфигурировать как выходы */
   DDRB = 0x3f;		   /* 5 выводов порта B сконфигурировать как выходы */
   DDRC = 0x03;		   /* 2 вывода порта C сконфигурировать как выходы */
	
	int reset(void)
	{
		PORTC = 0xff;
		PORTD = 0xff;
		PORTB = 0xff;
		return 0;
	}
	

	port vivod(int num, int diod)
	{
		port pt;
		switch(num){
			case 1:

				switch( diod )
				{	
					case 1:
					//PORTD = 0b11111110;
					port.pd=0b11111110;
						return 0;
					
					case 2:
					//PORTB = 0b11111110;
					port.pb=0b11111110;
					return 0;
					
					case 3:
					//PORTB = 0b11111011;
					port.pb=0b11111011;
					return 0;
					
					case 4:
					//PORTB = 0b11110111;
					port.pb=0b11110111;
					return 0;
					
					case 5:
					//PORTD = 0b11111011;
					port.pd=0b11111011;
					return 0;
					
					case 6:
					PORTD = 0b11111101;
					return 0;
					
					case 7:
					PORTB = 0b11111101;
					return 0;
					
					case 8:
					PORTD = 0b01111111;
					return 0;
				}
			case 2:
				switch( diod )
				{	
					case 1:
					PORTD = 0b11110111;
					return 0;
					
					case 2:
					PORTB = 0b11011111;
					return 0;
					
					case 3:
					PORTC = 0b11111110;
					return 0;
					
					case 4:
					PORTC = 0b11111101;
					return 0;
					
					case 5:
					PORTD = 0b10111111;
					return 0;
					
					case 6:
					PORTD = 0b11101111;
					return 0;
					
					case 7:
					PORTD = 0b11011111;
					return 0;
					
					case 8:
					PORTB = 0b11101111;
					return 0;
				}

			 
		}
		return 0;
		}
	
	

		while(1)
		{
			reset();
			vivod(1,6); vivod(2,6);
			_delay_ms(50);
			
//			vivod(1,7); vivod(2,7);	

			reset();
			vivod(1,5); vivod(2,1);
			_delay_ms(50);
			
//			vivod(1,7); vivod(2,7);	

			reset();
			vivod(1,4); vivod(2,2);
			_delay_ms(50);
			
//			vivod(1,7); vivod(2,7);

			reset();
			vivod(1,3); vivod(2,3);
			_delay_ms(50);
			
//			vivod(1,7); vivod(2,7);

			reset();
			vivod(1,2); vivod(2,4);
			_delay_ms(50);
			
//			vivod(1,7); vivod(2,7);

			reset();
			vivod(1,1); vivod(2,5);
			_delay_ms(50);
			
//			vivod(1,7); vivod(2,7);
//			_delay_ms(50);
		}
	}
