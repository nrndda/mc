  #include <avr/io.h>
   #include <util/delay.h>
   #define F_CPU 8000000UL  // 8 MHz

   int reset(void)
	{
		PORTC = 0xff;
		PORTD = 0xff;
		return 0;
	}

	int vivod(int *port, int zahl)
	{
		switch (zahl)
		{
			case 1:
			port=0b11011110;
			if (*port==&PORTC && ((port & 0b00010000) ==0b00000000))
			{
				PORTC=
			}
			break;

			case 2:
			port=0b10100100;
			break;

			case 3:
			port=0b10001100;
			break;

			case 4:
			port=0b10011010;
			break;

			case 5:
			port=0b10001001;
			break;

			case 6:
			port=0b10000001;
			break;

			case 7:
			port=0b11011100;
			break;

			case 8:
			port=0b10000000;
			break;

			case 9:
			port=0b10001000;
			break;

			case 0:
			port=0b11000000;
			break;

		}
		return 0;
	}

   int main(void)              // начало основой программы
   {
   
   DDRD = 0xff;            /* все выводы порта D сконфигурировать как выходы */
   DDRB = 0x01;		   /* 1 вывод порта B сконфигурировать как выходы, остальные как входы */
   DDRC = 0xff;		   /* все выводы порта C сконфигурировать как выходы */

//	int prev=0,k=0;


/*	void on(int *P, int n)
	{
		*P &= !(1<<n);
 	}
	
	void off(int *P, int n)
	{
		*P |= 1<<n;
 	}	*/
		
	
        while (1) {              // Бесконечный цикл

		
		/*	if( PORTB==0x01 && prev==0 )
			{
				k=!k;
				prev=1;
			}
			if(PORTB==0x01 && prev==1)
			{
				prev=0;
			}
			if(k)
			{ */
				PORTD=0x00; PORTC=0x00; PORTB=!0x11; _delay_ms(150);PORTB=0x11; PORTD=0xff; PORTC=0xff;
				vivod(&PORTD,4);
				_delay_ms(150);
				vivod(&PORTD,8);
				_delay_ms(150);
				vivod(&PORTC,1);
				vivod(&PORTD,5);
				_delay_ms(150);
				vivod(&PORTC,1);
				vivod(&PORTD,6);
				_delay_ms(150);
				vivod(&PORTC,2);
				vivod(&PORTD,3);
				_delay_ms(150);				
				vivod(&PORTC,4);
				vivod(&PORTD,2);
				_delay_ms(150);

		//	}

		}	
		
		return 0;

		       // закрывающая скобка бесконечного цикла

   }      // закрывающая скобка основной программы

