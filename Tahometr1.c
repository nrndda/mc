   #include <avr/io.h>
   #include <util/delay.h>
   #define F_CPU 4000000UL  // 4 MHz

	unsigned char rejim = 0;
	unsigned char min =0,hour = 0;

	int vivod(int segment, int zahl)
	{
		switch (zahl)
		{
			case 1:
			PORTD=0b10110111;
			break;

			case 2:
			PORTD=0b11000001;
			break;

			case 3:
			PORTD=0b10010001;
			break;

			case 4:
			PORTD=0b10110100;
			break;

			case 5:
			PORTD=0b10011000;
			break;

			case 6:
			PORTD=0b10001000;
			break;

			case 7:
			PORTD=0b10110011;
			break;

			case 8:
			PORTD=0b10000000;
			break;

			case 9:
			PORTD=0b10010000;
			break;

			case 0:
			PORTD=0b10000010;
			break;

			default:
			PORTD=0b10000000;
			break;

		}

		switch (segment)
		{
			case 1:
			PORTB = 0b11111110;
			break;

			case 2:
			PORTB = 0b11111101;
			break;

			case 3:
			PORTB = 0b11111011;
			break;

			case 4:
			PORTB = 0b11110111;
			break;

			case 5:
			PORTB = 0b11111111;
			break;
		
			default:
			PORTB = 0b11110000;
			break;
		}
		return 0;
	}
   
	interrupt [TIM1_COMP] void timer1(void)
		{
			
			int me=min%10;
			int md=(min-min%10)/10;
			int he=hour%10;
			int hd=(hour-hour%10)/10;
			min+=1;
			if (min == 60)
			{
				hour+=1;
				min=0;
			if (hour == 24)
			{
				hour = 0;
			}
			
			if (rejim == 0)
			{
				vivod(5,0);
				vivod(4,me);       
       				_delay_ms(1);
			//	vivod(0,5);
				vivod(3,md);
		       		_delay_ms(1);
			//	vivod(0,5);
		       		vivod(2,he);       
		       		_delay_ms(1);
			//	vivod(0,5);
				vivod(1,hd);
		      		_delay_ms(1);
			}
		}

   int main(void)             
   {
   	DDRD = 0xff;            
  	DDRB = 0xff;
  	DDRC = 0x00;
  	PORTC = 0xff;
	unsigned char B0Pressed = 0; 
	unsigned char B0Holded = 0;
//	unsigned int t=0;
//	unsigned char B1Pressed = 0;  
//	PORTD = 0x00;
//	PORTB = 0x00;
	MCUCR=0b10110000 // Power-save mode. Timer/Counter2 on
	vivod(5,0);
       while (1) 
	{      
	/*t+=1;  
   	if ( t== 3480 ) 		// Часы отстают 1сек в 4 минуты Надо попробовать 3479. Изучить прерывание и таймер!
	{
		t=0;
	 	min+=1;
	if (min == 60)
	{
		hour+=1;
		min=0;
	if (hour == 24)
	{
		hour = 0;
	}
	}
	}
	int me=min%10;
	int md=(min-min%10)/10;
	int he=hour%10;
	int hd=(hour-hour%10)/10;
	if (B0Holded + B0Pressed != 0)
	{
		t=25-25;
	}   
	else
	{
		t=25;
	} 

	if (rejim == 0)
	{
		vivod(5,0);
		vivod(4,me);       
       		_delay_ms(1);
	//	vivod(0,5);
		vivod(3,md);
       		_delay_ms(1);
	//	vivod(0,5);
       		vivod(2,he);       
       		_delay_ms(1);
	//	vivod(0,5);
		vivod(1,hd);
      		_delay_ms(1);
	}

	if (rejim == 1)
	{
       		_delay_ms(20);
		PORTB=0b11111100;        
       		_delay_ms(20);
		PORTB=0b11111111;       
	}
	if (rejim == 2)
	{
       		_delay_ms(20);
		PORTB=0b11110011;        
       		_delay_ms(20);
		PORTB=0b11111111;       
	}*/

	if (B0Pressed == 1) 
	    {                   		
	        B0Pressed = 0;
		min+=1;
/*	if (rejim == 1)
	{
	min++;
	}
	if (rejim == 2)
	{
	hour++;
	} */
    	    }

	if (B0Holded == 1) 
	    {                   		
	        B0Holded = 0;
		hour++;
		if (hour >= 24)
		{
			hour = 0;
		}
		vivod(5,0);
		vivod(4,me);       
       		_delay_ms(1);
	//	vivod(0,5);
		vivod(3,md);
       		_delay_ms(1);
	//	vivod(0,5);
       		vivod(2,he);       
       		_delay_ms(1);
	//	vivod(0,5);
		vivod(1,hd);
      		_delay_ms(1);
		//rejim++;
		//if (rejim >2) { rejim = 0; }
      		//while ((PINC & (1 << PC0)) == 0) { }
    	    }

    	if (B0Holded || B0Pressed == 0)
    	{
      		if ((PINC & (1 << PC0)) == 0)      //Фиксирует нажатие
     		{
        		_delay_ms(5);        
        		if ((PINC & (1 << PC0)) == 0)    //Проверяет нажатие
        		{
			_delay_ms(50);
			if ((PINC & (1 << PC0)) == 0)    //Проверяет нажатие
			{
          			B0Holded = 1;  //Устанавливает флаг "кнопка нажата"
			}
			else
			{
          			B0Pressed = 1;  //Устанавливает флаг "кнопка нажата"
			}
			}
      		}    
    	}

	}       

	return 0;
   }

