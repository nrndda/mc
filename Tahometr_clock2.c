   #include <avr/io.h>   
   #include <util/delay.h>
   #include <avr/interrupt.h>
   #include <avr/mega.h>
   #define F_CPU 4915200UL  // 4 MHz

	unsigned char rejim = 0,fb = 0,k = 0,sreg;
	unsigned char sec = 0;
	unsigned int turn=0,t = 0,d = 0, refresh=400;
	
	eeprom unsigned char max_speed,min,hour;
	int me,md,he,hd;
	float speed=0;
	
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
		d+=1;
		if (d == refresh)
		{
		speed = 7.35133 * (float)turn / 2.0;
		if ((unsigned char)speed>max_speed) max_speed=(unsigned char)speed;
		turn = 0; d = 0;
		}
		if (t >= 485)
		{
			if ((min%20 == 0) & (sec == 0)) sec+=1; //Корекция
			sec+=1;
			t=0;
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
//	unsigned char B1Pressed = 0;  
	vivod(5,0,0);
//	ACSR = 0x80; // Analog Comparator off
//	ADCSRA = 0x00; // Analog-to-Digital Converter off
	GIMSK = 0x00;
	TCCR0 = 0b000000010;	//Timer0 clk/1
	TCNT0 = 0x00;		//Timer0 start point
	TIMSK = (1 << TOIE0);	//Timer0 on
//	MCUCR= 0x00;	// Idle 		// 0b10110000 - Power-save mode. Timer/Counter2 on
	sei();
 	while(1)
	{
	while (((PINC & 0b00000001) == 1) && (((PINC & 0b00000010) == 0) || ((fb == 1) && ((PINC & 0b00000010) == 2)))) 
	{
		// if (rejim == 0) MCUCR= 0x00;	// Idle 		// 0b10110000 - Power-save mode. Timer/Counter2 on
		if (rejim == 0)
		{
			unsigned int speedd=speed*100;
			unsigned int speed1=speedd%10;
			speedd=(speedd-speed1)/10;
			unsigned int speed2=speedd%10;
			speedd=(speedd-speed2)/10;
			unsigned int speed3=speedd%10;
			speedd=(speedd-speed3)/10;
			unsigned int speed4=speedd%10;
			vivod(4,speed1,0);
			_delay_ms(1);
			vivod(3,speed2,0);
			_delay_ms(1);
			vivod(2,speed3,1);
			_delay_ms(1);
			vivod(1,speed4,0);
			_delay_ms(1);

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
		if (rejim == 4)
		{
			vivod(2,max_speed%10,0);
			vivod(1,(max_speed-max_speed%10)/10,0);
		}
	if (((PINC & 0b00000010) == 0) && (fb==1)) fb=0;
	}
	vivod(5,12,0);
 	

        if ((PINC & 0b00000001) == 0)    //Проверяет нажатие
	{
		_delay_ms(50);
		if ((PINC & 0b00000001) == 0)    //Проверяет нажатие
		{
      			B0Holded = 1;  //Устанавливает флаг "кнопка нажата"
		}
		else
		{
       			B0Pressed = 1;  //Устанавливает флаг "кнопка нажата"
		}
	}

	if ((PINC & 0b00000010) == 2)    //Проверяет нажатие
	{
       			turn+=1;  //Добавляет оборот
			fb = 1;
	} 

	if (B0Pressed == 1) 
	{                   		
	        B0Pressed = 0;
		if (rejim == 0)
		{
			refresh+=50;
			if (refresh >700) refresh=200;
		}
		if ((rejim == 1) || (rejim == 4)) continue;
		if (rejim == 2)	min++;
		if (rejim == 3) hour++;
        }

	if (B0Holded == 1) 
	{                   		
		rejim++;
		if (rejim >4) rejim = 0;
      		while ((PINC & (1 << PC0)) == 0) { }
		B0Holded = 0;
    	}

	}
	return 0;
   } 

