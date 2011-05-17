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
   char sreg, time_position;
   char min = 0,hour = 0, sec = 0;
   unsigned int time = 0;
   char * string;
   
	void USART_Init( unsigned int baud )
	{
			/* Set baud rate */
		sreg=SREG;
		cli();
		UBRRH = (unsigned char)(baud>>8);
		UBRRL = (unsigned char)baud;
			/* Enable Receiver and Transmitter */
		UCSRB = (1<<RXEN)|(1<<TXEN);
			/* Set frame format: 8data, 2stop bit */
		UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);
		SREG=sreg;
	 }

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
	}

	void Time_disp(char position)
	{
		WriteCommand(position);		// Установка начального адреса
		WriteByte(0x30+hour/10);
		WriteByte(0x30+hour%10);
		WriteByte(0x20);			// Пробел
		WriteByte(0x30+min/10);
		WriteByte(0x30+min%10);
	}

	void My_disp(void)
	{
	 	if (sec == 60) 
		{
			min+=1;
			sec=0;
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
		WriteCommand(0b00001100); // Ничего не мигает
		WriteCommand(time_position-1);
		WriteByte(0x93);
		Time_disp(time_position);
	}

	SIGNAL (SIG_OVERFLOW0)		/* Обработка прерывания от переполнения счётчика 0-ого таймера */
	{
		time++;
		if (time == 31250)
		{
			sec+=1;
			time=0;
		}
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
		//MCUCR = 0b00000001;	/* Прерывание от INT0 генерируется изменением напряжения, а на INT1 низким напряжением */
		//GICR  = 0b01000000;
		sei();
		LCD_init();
		WriteCommand(0b00001100);		// Включение дисплея без курсора, ничего не мигает
		string = (char *)malloc((D_SZ+1)*sizeof(char));
		time_position = 0b10000000+1;
 		while(1)
		{	
			strcpy(string," Smax,R:");
			WriteString(0b11000000,string);
//			WriteCommand(0b11000000);	// Установка начального адреса на начало второй строки
			My_disp();
		}
		free(string);
		return 0;
   	} 