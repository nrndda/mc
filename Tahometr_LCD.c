   #include <avr/io.h>
   #include <util/delay.h>
   #include <avr/interrupt.h>
   #include <stdlib.h>
   #include <string.h>
/* RS_0 RW_1 E_2 Bit1_3 Bit2_4 Bit3_5 Bit4_6*/
	#define RS_1  PORTC  = (PORTC & 0b11111110) | 0b00000001
	#define RW_1 PORTC  = (PORTC & 0b11111101) | 0b00000010
	#define E_1  PORTC  = (PORTC & 0b11111011) | 0b00000100
	#define RS_0  PORTC &= 0b11111110
	#define RW_0 PORTC &= 0b11111101
	#define E_0  PORTC &= 0b11111011
	#define D_SZ 16				//Display string size
	char sreg, max_speed = 0,EEPROM_max_speed,EEPROM_range, time_position;
	char min = 0,hour = 0, sec = 0,flash=0,refresh = 4;
	char light=0;
	unsigned int time = 0, B0Pressed = 0, B0Holded = 0;
	unsigned int B1Pressed = 0, B1Holded = 0;
	unsigned int B2Pressed = 0, B2Holded = 0;
	unsigned int B3Pressed = 0, B3Holded = 0;
	double speed=0.0, turn=0.0, range=0.0;
	char * string;

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

	void WriteCommand(char data) //запись 1-го байта комманды по текущему адресу
	{
		RS_0;
		RW_0;
		_delay_ms(0.5);
  		PORTC = (PORTC & 0b00000111) | ((data >> 1) & 0b01111000);
 		_delay_ms(0.1);
  		E_1;
 		_delay_ms(0.5);
 		E_0;
 		_delay_ms(0.1);
  		PORTC = (PORTC & 0b00000111) | ((data << 3)& 0b01111000);
		_delay_ms(0.1);
		E_1;
 		_delay_ms(0.5);
  		E_0;
		RW_1;
		PORTC = (PORTC & 0b00000111);
	}

	void WriteByte(char data) //запись 1-го байта данных по текущему адресу
	{
		RS_1;
		RW_0;
		_delay_ms(0.5);
		PORTC = (PORTC & 0b00000111) | ((data & 0b11110000) >> 1);
		_delay_ms(0.1);
		E_1;
		_delay_ms(0.5);
		E_0;
		PORTC = (PORTC & 0b00000111) | ((data & 0b00001111) << 3);
		_delay_ms(0.1);
		E_1;
		_delay_ms(0.5);
		E_0;
		RW_1;
		PORTC = (PORTC & 0b00000111);
	}

	void WriteString(char position, char * string)
	{
		WriteCommand(position);
		for (int i=0;i<=D_SZ;i++)
		{
			WriteByte(*(string + i));
		}
	}

	void LCD_init(void)
	{
		_delay_ms(30);
		PORTC = 0b00011000;		//DB5=1 DB4=1
 		_delay_ms(1);
  		E_1;
 		_delay_ms(5);
 		E_0;
		RW_1;
 		_delay_ms(10);
		PORTC = 0b00011000;		//DB5=1 DB4=1
 		_delay_ms(1);
  		E_1;
 		_delay_ms(5);
 		E_0;
		RW_1;
 		_delay_ms(10);
		PORTC = 0b00011000;		//DB5=1 DB4=1
		 		_delay_ms(1);
  		E_1;
 		_delay_ms(5);
 		E_0;
		RW_1;
 		_delay_ms(10);

		PORTC = 0b00010000;		//DB5=1
		_delay_ms(1);
  		E_1;
 		_delay_ms(5);
 		E_0;
		RW_1;
 		_delay_ms(10);

		WriteCommand(0b00101000);
 		_delay_ms(10);
		WriteCommand(0b00001000);
 		_delay_ms(10);
		WriteCommand(0b00000001);
 		_delay_ms(10);
		WriteCommand(0b00000110);
 		_delay_ms(10);
/*		PORTC = 0b00010000;		//DB5=1
		_delay_ms(20);
		PORTC = 0b01000000;		//DB7=1
		_delay_ms(20);

		PORTC = 0b00000000;
		_delay_ms(20);
		PORTC = 0b01000000;		//DB7=1
		_delay_ms(20);

		PORTC = 0b00000000;
		_delay_ms(20);
		PORTC = 0b00001000;		//DB4=1
		_delay_ms(20);

		PORTC = 0b00000000;
		_delay_ms(20);
		PORTC = 0b00110000;		//DB6=1 DB5=1

		RS_0;RW_0;E_1;
		_delay_ms(60);
		PORTC = 0b00000000;
		_delay_ms(20);
		PORTC = 0b01111000;
		_delay_ms(20);
		RW_1; E_0; RS_1;*/
	}

	void Time_disp(char position)
	{
		WriteCommand(position);		// Установка начального адреса
		WriteByte(0x30+hour/10);
		WriteByte(0x30+hour%10);
		WriteByte(0x20);			// Пробел
		WriteByte(0x30+min/10);
		WriteByte(0x30+min%10);

		if (flash)
		{
			WriteCommand(time_position + 2);
			WriteByte(0x3A);			// Двоеточие
			_delay_ms(50);
			flash=0;
		}
	}

	void Dist_disp(char position)
	{
		int temp = (int) (range*100);
		int fractional = temp%100;
		int whole =(temp - fractional)/100;

		WriteCommand(position);		// Установка начального адреса
		WriteByte(0x30+(char) (whole/10));
		WriteByte(0x30+(char) (whole%10));
		WriteByte(0x2E);			// Точка
		WriteByte(0x30+(char) (fractional/10));
		WriteByte(0x30+(char) (fractional%10));
	}

	void Speed_disp(char position)
	{
		int temp1 = (int) (speed*100);
		char temp = (char) (temp1 - temp1%100)/100;
		WriteCommand(position);		// Установка начального адреса
		WriteByte(0x30+temp/10);
		WriteByte(0x30+temp%10);
	}

	void Time_adjusting(void)
	{
		WriteCommand(time_position-1);
		WriteByte(0x84);
		_delay_ms(50);
	}
	void My_disp(void)
	{
		WriteCommand(0b00001100); // Ничего не мигает
		WriteCommand(time_position-1);
		WriteByte(0x93);
		Time_disp(time_position);
		WriteCommand(time_position+5);
		WriteByte(0x3B);			// Точка с запятой
		Dist_disp(time_position+6);
		WriteCommand(time_position+11);
		WriteByte(0x3B);			// Точка с запятой
		Speed_disp(time_position+12);
	}
	void Buttons(void)
	{
		char tmp=0;
		if ((PIND & 0b10000000) == 0)    //Проверяет нажатие
		{
			_delay_ms(10);
			if ((PIND & 0b10000000) == 0)    //Проверяет нажатие
      		{
				_delay_ms(150);
				if ((PIND & 0b10000000) == 0)    //Проверяет нажатие
      			{
					B3Holded = 1;  //Устанавливает флаг "кнопка нажата"
					PORTB = 1;
					while ((PIND & 0b10000000) == 0) My_disp();
				}
				else	B3Pressed = 1;  //Устанавливает флаг "кнопка нажата"
			}
		}

		if ((PIND & 0b01000000) == 0)    //Проверяет нажатие
		{
			_delay_ms(10);
			if ((PIND & 0b01000000) == 0)    //Проверяет нажатие
      		{
				_delay_ms(150);
				if ((PIND & 0b01000000) == 0)    //Проверяет нажатие
      			{
					B2Holded = 1;  //Устанавливает флаг "кнопка нажата"
					while ((PIND & 0b01000000) == 0) My_disp();
				}
				else	B2Pressed = 1;  //Устанавливает флаг "кнопка нажата"
			}
		}

		if ((PIND & 0b00100000) == 0)    //Проверяет нажатие
		{
			_delay_ms(10);
			if ((PIND & 0b00100000) == 0)    //Проверяет нажатие
      		{
				_delay_ms(150);
				if ((PIND & 0b00100000) == 0)    //Проверяет нажатие
    	  		{
					B1Holded = 1;  //Устанавливает флаг "кнопка нажата"
					while ((PIND & 0b00100000) == 0) My_disp();
				}
				else	B1Pressed = 1;  //Устанавливает флаг "кнопка нажата"
			}
		}

		if ((PIND & 0b00010000) == 0)    //Проверяет нажатие
		{
			_delay_ms(10);
			if ((PIND & 0b00010000) == 0)    //Проверяет нажатие
      		{
				_delay_ms(150);
				if ((PIND & 0b00010000) == 0)    //Проверяет нажатие
      			{
					B0Holded = 1;  //Устанавливает флаг "кнопка нажата"
					while ((PIND & 0b00010000) == 0) My_disp();
				}
				else	B0Pressed = 1;  //Устанавливает флаг "кнопка нажата"
			}
		}


		if ((!light)&(B0Pressed|B1Pressed|B2Pressed|B3Pressed|B0Holded|B1Holded|B2Holded|B3Holded))
		{
			PORTB = 1;
 		}

		if (B0Holded)
		{
			B0Holded = 0;
			if (light)   light = 0;
			else light = 1;
			if (light) PORTB = 1;
			else PORTB = 0;
		}

		if (B0Pressed)
		{
			B0Pressed = 0;
			WriteCommand(time_position-1);
			WriteByte(0x87);
			_delay_ms(150);
			WriteCommand(time_position-1);
			WriteByte(0x20);
			if (!light) PORTB =0;
			if (refresh<10)	  refresh++;
			else refresh = 2;
		}

		if (B1Pressed)
		{
			B1Pressed = 0;
			if (!light) PORTB =0;
		}

		if (B2Pressed)
		{
			B2Pressed = 0;
			if (!light) PORTB =0;
		}

		if (B3Pressed)
		{
			B3Pressed = 0;
			if (!light) PORTB =0;
		}

		if (B1Holded)
		{
			B1Holded = 0;
			if (!light) PORTB =0;
		}

		if (B2Holded)
		{
			B2Holded = 0;
			if (!light) PORTB =0;
		}

		if (B3Holded)
		{
			B3Holded = 0;
			tmp=min;
			Time_adjusting();
			while (B3Holded==0)
			{
				if ((PIND & 0b10000000) == 0)    //Проверяет нажатие
				{
					_delay_ms(300);
					if ((PIND & 0b10000000) == 0)    //Проверяет нажатие
					{
						B3Holded = 1;  //Устанавливает флаг "кнопка нажата"
						while ((PIND & 0b10000000) == 0) My_disp();
						continue;
					}
					//else	B3Pressed = 1;  //Устанавливает флаг "кнопка нажата"
				}
				if ((PIND & 0b01000000) == 0)    //Проверяет нажатие
				{
					_delay_ms(10);
					if ((PIND & 0b01000000) == 0)    //Проверяет нажатие
					{
						//B2Holded = 1;  //Устанавливает флаг "кнопка нажата"
						_delay_ms(100);
						B2Pressed = (PIND & 0b01000000);
						while ((PIND & 0b01000000) == 0)
						{
							Time_adjusting();
							My_disp();
							if (hour==0) hour=24;
							hour--;
							tmp=min;
						}
					}

				}
				if (B2Pressed)
				{
					if (hour==0) hour=24;
					hour--;
					tmp=min;
					B2Pressed = 0;
				}
				if ((PIND & 0b00100000) == 0)    //Проверяет нажатие
				{
					_delay_ms(10);
					if ((PIND & 0b00100000) == 0)    //Проверяет нажатие
					{
						//B2Holded = 1;  //Устанавливает флаг "кнопка нажата"
						_delay_ms(100);
						B1Pressed = (PIND & 0b00100000);
						while ((PIND & 0b00100000) == 0)
						{
							Time_adjusting();
							My_disp();
							if (hour>=24)
							{
								hour=0;
								continue;
							}
							hour++;
							tmp=min;
						}
					}
				}
				if (B1Pressed)
				{
					if (hour>=24)
					{
						hour=0;
						continue;
					}
					hour++;
					tmp=min;
					B1Pressed = 0;
				}
				Time_adjusting();
				My_disp();
				if (min!=tmp)
				if ((min-tmp>1) | (tmp-min>1)) return;
			}
			B3Holded=0;
//			WriteCommand(time_position+3);
			tmp=min;
			while (B3Holded==0)
			{
				if ((PIND & 0b10000000) == 0)    //Проверяет нажатие
				{
					_delay_ms(300);
					if ((PIND & 0b10000000) == 0)    //Проверяет нажатие
					{
						B3Holded = 1;  //Устанавливает флаг "кнопка нажата"
						while ((PIND & 0b10000000) == 0)
						{
						  Time_adjusting();
						  My_disp();
						}
						continue;
					}
					//else	B3Pressed = 1;  //Устанавливает флаг "кнопка нажата"
				}
				if ((PIND & 0b01000000) == 0)    //Проверяет нажатие
				{
					_delay_ms(10);
					if ((PIND & 0b01000000) == 0)    //Проверяет нажатие
					{
						//B2Holded = 1;  //Устанавливает флаг "кнопка нажата"
						_delay_ms(100);
						B2Pressed = (PIND & 0b01000000);
						while ((PIND & 0b01000000) == 0)
						{
							Time_adjusting();
							My_disp();
							if (min<=0) min=60;
							min--;
							tmp=min;
						}
					}
				}

				if (B2Pressed)
				{
					if (min<=0) min=60;
					min--;
					tmp=min;
					B2Pressed=0;
				}

				if ((PIND & 0b00100000) == 0)    //Проверяет нажатие
				{
					_delay_ms(10);
					if ((PIND & 0b00100000) == 0)    //Проверяет нажатие
					{
						//B2Holded = 1;  //Устанавливает флаг "кнопка нажата"
						_delay_ms(100);
						B1Pressed = (PIND & 0b00100000);
						while ((PIND & 0b00100000) == 0)
						{
							Time_adjusting();
							My_disp();
							if (min>=59)
							{
								min=0;
								continue;
							}
							min++;
							tmp=min;
						}
					}
				}
				if (B1Pressed)
				{
					if (min>=59)
					{
						min=0;
						continue;
					}
					min++;
					tmp=min;
					B1Pressed = 0;
				}
				Time_adjusting();
				My_disp();
				if (min!=tmp)
				if ((min-tmp>1) | (tmp-min>1)) B3Holded=1;
			}
			if (!light) PORTB =0;
		}
		B3Holded = 0; // Для предотвращения повторного захода в обработку кнопки
	}

	SIGNAL (SIG_OVERFLOW0)		/* Обработка прерывания от переполнения счётчика 0-ого таймера */
	{
		time++;
		if (time == 31250)
		{
			sec+=1;
			flash=1;
			time=0;
			if (sec%refresh == 0)
			{
				speed = 2.37744 * turn / refresh;
				if ((unsigned char)speed>max_speed) max_speed=(unsigned char)speed;
				turn = 0;
			}
		}
		if (sec == 60)
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
		if (min == 60)
		{
			hour+=1;
			min=0;
		}
		if (hour == 24)
		{
			hour = 0;
		}

	}

	SIGNAL (SIG_INTERRUPT0)		/* Обработка прерывания от индукционного датчика*/
	{
		turn+=0.5;
		range+= 0.3302*0.001;
	}

	int main(void)
	{
		DDRC = 0xff;		/* Порты для управления LCD-дисплеем */
		PORTC = 0x00;
		DDRD = 0x00;
		PORTD = 0xff;
		DDRB = 1; // Порт PD0 как вывод
		PORTB = 0x00;
		//	SFIOR = (SFIOR & !0x04);
		//	ACSR = 0x80; // Analog Comparator off
		//	ADCSRA = 0x00; // Analog-to-Digital Converter off
		GIMSK = 0x00;
		TCCR0 = 0b00000001;	//Timer0 clk/1
		TCNT0 = 0x00;		//Timer0 start point
		TIMSK = (1 << TOIE0);	//Timer0 on
		MCUCR = 0b00000001;	/* Прерывание от INT0 генерируется изменением напряжения, а на INT1 низким напряжением */
		GICR  = 0b01000000;
		sei();
		LCD_init();
		WriteCommand(0b00001100);		// Включение дисплея без курсора, ничего не мигает
		string = (char *)malloc((D_SZ+1)*sizeof(char));
		time_position = 0b10000000+1;
 		while(1)
		{
			Buttons();
			if ((min == 0)&&(sec == 10))
			{
				EEPROM_range = 0.01*EEPROM_read(5)+EEPROM_read(3);
				EEPROM_max_speed = EEPROM_read(1);
			}
			strcpy(string," Smax,R:");
			WriteString(0b11000000,string);
			WriteByte(0x30+EEPROM_max_speed);
			WriteByte(0x3B);			// Точка с запятой
			WriteByte(0x30+EEPROM_range);
//			WriteCommand(0b11000000);	// Установка начального адреса на начало второй строки
			My_disp();
		}
		free(string);
		return 0;
   	}

