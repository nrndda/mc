   #include <avr/io.h>   
   #include <util/delay.h>
   #include <avr/interrupt.h>
   #define F_CPU 4915200UL  // 4 MHz

	unsigned char rejim = 0,fb = 0,k = 0,sreg,l=0 , max_speed=0;
	unsigned char min =0,hour = 0, sec = 0;
	unsigned int t = 0,d = 0;
	int me,md,he,hd;
	float speed=0.0, turn=0.0, range=0.0, refresh=2.0;
	
	void EEPROM_write(unsigned int uiAddress, unsigned char ucData)
	{
		 	/* Wait for completion of previous write */
		 while(EECR & (1<<EEWE));
		 	/* Set up address and data registers */
		 EEAR = uiAddress;
		 EEDR = ucData;
		 sreg=SREG;
		 cli();
 		 	/* Write logical one to EEMWE */
 		 EECR |= (1<<EEMWE);
 		 	/* Start eeprom write by setting EEWE */
 		 EECR |= (1<<EEWE);
		 SREG=sreg;
	}

	unsigned char EEPROM_read(unsigned int uiAddress)
	{
 		 	/* Wait for completion of previous write */
 		 while(EECR & (1<<EEWE));
		 	/* Set up address register */
		 EEAR = uiAddress;
		 sreg=SREG;
		 cli();
 		 	/* Start eeprom read by writing EERE */
		 EECR |= (1<<EERE);
		 	/* Return data from data register */
		 SREG=sreg;
		 return EEDR;
	}


	
	int vivod(int segment, int zahl, char point)
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

			case 12:
			PORTD=0b11111111;
			break;	

			default:
			PORTD=0b10000000;
			break;

		}
		if (point) PORTD&=0b01111111;
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

	void vivod_float(float temp)
	{
		unsigned int a;
		temp=temp*100;
		a=(unsigned int)temp;
		unsigned int a1=(unsigned int)(a%10);
		a=(a-a1)/10;
		unsigned int a2=(unsigned int)(a%10);
		a=(a-a2)/10;
		unsigned int a3=(unsigned int)(a%10);
		a=(a-a3)/10;
		unsigned int a4=(unsigned int)(a%10);
		vivod(4,a1,0);
		_delay_ms(1);
		vivod(3,a2,0);
		_delay_ms(1);
		vivod(2,a3,1);
		_delay_ms(1);
		vivod(1,a4,0);
		_delay_ms(1);
	}
 
  	int time(char a)
	{
		me=min%10;
		md=(min-min%10)/10;
		he=hour%10;
		hd=(hour-hour%10)/10;
		vivod(4,me,0);       
		_delay_ms(1);
		vivod(3,md,0);
		_delay_ms(1);
		vivod(2,he,a);       
		_delay_ms(1);
		vivod(1,hd,0);
		_delay_ms(1);   
		return 0;
	}

	SIGNAL (SIG_OVERFLOW0)
	{
		t+=1;
		if (((PINC & 0b00000010) == 2) && (fb==0))    //Проверяет нажатие
		{
       			turn+=0.5;  //Добавляет оборот
			fb = 1;
		} 
		if (((PINC & 0b00000010) == 0) && (fb==1)) fb=0;

		if (t >= 485)
		{
			if ((min%20 == 0) && (sec == 0)) sec+=1; //Корекция
			sec+=1;
			t=0;
			d+=1;
			if ((float)d >= refresh)
			{
				speed = 6.33 * turn / refresh;              // Поправил коэффициент деления!!!!!!!!!!!!
				range = range + 1.76 * 0.001 * turn;
				if ((unsigned char)speed>max_speed) max_speed=(unsigned char)speed;
				turn = 0; d = 0;
			}
		}
		if (sec >= 60) 
		{
			min+=1;
			sec=0;
			if (min%10 == 0) 
			{
				if (max_speed>EEPROM_read(1))	EEPROM_write(1,max_speed);
				unsigned char gg=EEPROM_read(5)+(unsigned char)(((unsigned int)(range*100.0))%100);
				EEPROM_write(5,gg);
				gg=EEPROM_read(3)+(unsigned char)((( ((unsigned int)(range*100.0) - ((unsigned int)(range*100.0))%100) /100))%100);
				EEPROM_write(3,gg);
			}	
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
//	unsigned char B1Pressed = 0;  
	vivod(5,0,0);
//	ACSR = 0x80; // Analog Comparator off
//	ADCSRA = 0x00; // Analog-to-Digital Converter off
	GIMSK = 0x00;
	TCCR0 = 0b000000010;	//Timer0 clk/8
	TCNT0 = 0x00;		//Timer0 start point
	TIMSK = (1 << TOIE0);	//Timer0 on
//	MCUCR= 0x00;	// Idle 		// 0b10110000 - Power-save mode. Timer/Counter2 on
	sei();
 	while(1)
	{
	while (l || (((PINC & 0b00000001) == 1) && (((PINC & 0b00000010) == 0) || ((fb == 1) && ((PINC & 0b00000010) == 2))))) 
	{
		// if (rejim == 0) MCUCR= 0x00;	// Idle 		// 0b10110000 - Power-save mode. Timer/Counter2 on
		if (rejim == 0)
		{
			vivod_float(speed);
		}		
		if (rejim == 1)
		{
			time(1);
		}	
		if (rejim == 2)
		{
			time(0);
			vivod(4,11,0); time(0); vivod(3,11,0);	
			time(0);      
		}
		if (rejim == 3)
		{
			time(0);
			vivod(1,11,0); time(0); vivod(2,11,0);	
       			time(0);      
		}
		if (rejim == 5)
		{
			int temp=EEPROM_read(1);
			vivod(4,temp%10,0);
			vivod(3,(temp-temp%10)/10,0);
			vivod(2,max_speed%10,0);
			vivod(1,(max_speed-max_speed%10)/10,0);
		}
		if (rejim == 4)
		{
			vivod_float(range);
		}
	//	if (((PINC & 0b00000010) == 0) && (fb==1)) fb=0;
		if ((l==1) && ((PINC & 0b00000001) == 1)) l=0;
	}
	vivod(5,12,0);

        if ((PINC & 0b00000001) == 0)    //Проверяет нажатие
	{
		_delay_ms(80);
		if ((PINC & 0b00000001) == 0)    //Проверяет нажатие
		{
      			B0Holded = 1;  //Устанавливает флаг "кнопка нажата"
		}
		else
		{
       			B0Pressed = 1;  //Устанавливает флаг "кнопка нажата"
		}
	}

/*	if (((PINC & 0b00000010) == 2) && !fb)    //Проверяет нажатие
	{
       			turn+=0.5;  //Добавляет оборот
			fb = 1;
	}  */

	if (B0Pressed == 1) 
	{                   		
	        B0Pressed = 0;
		if (rejim == 0)
		{
			refresh+=1;
			if (refresh >= 6) refresh=2;
		}
		if ((rejim == 1) || (rejim == 5)) continue;
		if (rejim == 2)	min++;
		if (rejim == 3) hour++;
		if (rejim == 4) 
		{
		unsigned char sec1=sec;
		while ((sec - sec1) <= 3) vivod_float((float)EEPROM_read(3) + (float)EEPROM_read(5)/100.0);
		}
        }

	if (B0Holded == 1) 
	{                   		
		rejim++;
		if (rejim >5) rejim = 0;
		B0Holded = 0;
		l = 1;
    	}

	}
	return 0;
   } 

