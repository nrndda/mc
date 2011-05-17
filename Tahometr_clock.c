   #include <avr/io.h>   
   #include <util/delay.h>
   #include <avr/interrupt.h>
   #define F_CPU 4915200UL  // 4 MHz

	unsigned char rejim = 0;
	unsigned char min =0,hour = 0, sec = 0;
	unsigned int speed=0,turn=0,t = 0;
	int me,md,he,hd;

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
			
			case 11:
			PORTD=0b01111111;
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
 
  	int time(void)
	{
		me=min%10;
		md=(min-min%10)/10;
		he=hour%10;
		hd=(hour-hour%10)/10;
		vivod(4,me);       
		_delay_ms(1);
		vivod(3,md);
		_delay_ms(1);
		vivod(2,he);       
		_delay_ms(1);
		vivod(1,hd);
		_delay_ms(1);   
		return 0;
	}

	SIGNAL (SIG_OVERFLOW0)
	{
		t+=1;
		if (t >= 485)
		{
			if ((min%20 == 0) & (sec == 0)) sec+=1;
			sec+=1;
			t=0;
			if (sec%2 == 0) 
			{
				speed = turn / 2;
				turn = 0;
			}
		}
		if (sec >= 60) 
		{
			min+=1;
			sec=0;
		}
		if (min >= 60)
		{
			hour+=1;
			min=0;
		}
		if (hour >= 24)
		{
			hour = 0;
		}
		
	}

   int main(void)             
   {
   	DDRD = 0xff;            
  	DDRB = 0xff;
  	DDRC = 0x00;
  	PORTC = 0xff;
//	SFIOR = (SFIOR & !0x04);
	unsigned char B0Pressed = 0; 
	unsigned char B0Holded = 0;
	unsigned char B1Pressed = 0;  
	vivod(5,0);
	ACSR = 0x80; // Analog Comparator off
	ADCSRA = 0x00; // Analog-to-Digital Converter off
	GIMSK = 0x00;
	TCCR0 = 0b000000010;	//Timer0 clk/1
	TCNT0 = 0x00;		//Timer0 start point
	TIMSK = (1 << TOIE0);	//Timer0 on
//	MCUCR= 0x00;	// Idle 		// 0b10110000 - Power-save mode. Timer/Counter2 on
	sei();
 m1:    while (((PINC & (1 << PC0)) == 1) || ((PINC & (1 << PC1)) == 1)) 
	{
		// if (rejim == 0) MCUCR= 0x00;	// Idle 		// 0b10110000 - Power-save mode. Timer/Counter2 on
		if (rejim == 0)
		{
			time();
		}	
		if (rejim == 1)
		{
			time();
			vivod(4,11); time(); vivod(3,11);	
			time();      
		}
		if (rejim == 2)
		{
			time();
			vivod(1,11); time(); vivod(2,11);	
       			time();      
		}
	}
	vivod(5,7);
        
        if ((PINC & (1 << PC0)) == 0)    //Проверяет нажатие
	{
		_delay_ms(70);
		if ((PINC & (1 << PC0)) == 0)    //Проверяет нажатие
		{
      			B0Holded = 1;  //Устанавливает флаг "кнопка нажата"
		}
		else
		{
       			B0Pressed = 1;  //Устанавливает флаг "кнопка нажата"
		}
	}
	if ((PINC & (1 << PC1)) == 0)    //Проверяет нажатие
	{
		_delay_ms(5);
		if ((PINC & (1 << PC1)) == 0)    //Проверяет нажатие
		{
       			turn+=1;  //Добавляет оборот
		}
	}
	else goto m1;

	if (B0Pressed == 1) 
	{                   		
	        B0Pressed = 0;
		if (rejim == 0) goto m1;
		if (rejim == 1)	min++;
		if (rejim == 2) hour++;
        }

	if (B0Holded == 1) 
	{                   		
		rejim++;
		if (rejim >2) rejim = 0;
      		while ((PINC & (1 << PC0)) == 0) { }
		B0Holded = 0;
    	}

	goto m1;
	return 0;
   } 

