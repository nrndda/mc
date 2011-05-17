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
	char sreg, time_position,change_c,change_h;
	char min = 0,hour = 0, sec = 0,flash=0;
	unsigned int time = 0;
	double cold=0.0, hot=0.0;
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
		if (sec >= 60) 
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
			_delay_ms(300);
			flash = 0;
		}
	}
	
	void Water_disp(char position)
	{
		char hundreds,decades,units,fractional,fractional1,fractional2;
		hundreds = (char) ( ((int) cold) / 100);
		decades = (char) ((((int) cold) % 100) / 10);
		units = (char) ( ((int) cold) % 10);
		fractional = (cold - hundreds * 100 - decades * 10 - units)*100;
		fractional1 = (char) (fractional / 10);
		fractional2 = (char) (((int) fractional) % 10);
		WriteCommand(position);		// Установка начального адреса
		WriteByte(0x58);
		WriteByte(0x3A);
//		WriteCommand(position+2);		// Установка начального адреса
		WriteByte(0x30+hundreds);
		WriteByte(0x30+decades);
		WriteByte(0x30+units);
		WriteByte(0x2C);
		WriteByte(0x30+fractional1);
		WriteByte(0x30+fractional2);
		WriteByte(0xA1);
		WriteByte(0x3A);
//		WriteCommand(position+7);		// Установка начального адреса
		hundreds = (char) ( ((int) hot) / 100);
		decades = (char) ((((int) hot) % 100) / 10);
		units = (char) ( ((int) hot) % 10);
		fractional = (hot - hundreds * 100 - decades * 10 - units)*100;
		fractional1 = (char) (fractional / 10);
		fractional2 = (char) (((int) fractional) % 10);
		WriteByte(0x30+hundreds);
		WriteByte(0x30+decades);
		WriteByte(0x30+units);
		WriteByte(0x2C);
		WriteByte(0x30+fractional1);
		WriteByte(0x30+fractional2);
	}
	
	void My_disp(void)
	{
		WriteCommand(0b00001100); // Ничего не мигает
		WriteCommand(time_position+5);
		WriteByte(0x85);
		WriteCommand(time_position-1);
		WriteByte(0x84);
		Time_disp(time_position);
		WriteCommand(0b11000000);	// Установка начального адреса на начало второй строки
		Water_disp(0b11000000);

	}

	ISR (SIG_OVERFLOW0)		/* Обработка прерывания от переполнения счётчика 0-ого таймера */
	{
		time++;
		if (time == 31250)
		{
			sec+=1;
			flash=1;
			time=0;
		}

	}
	ISR (SIG_INTERRUPT0)
	{
		if (change_c == 1) 
		{
			cold+=0.01;
			change_c = 0;
		}
		else change_c = 1;
		if (cold >= 1000.00) cold = 0;
	}
	ISR (SIG_INTERRUPT1)
	{
		if (change_h == 1) 
		{
		  hot+=0.01;
		  change_h = 0;
		}
		else change_h = 1;
		if (hot >= 1000.00) hot = 0;
	}

	int main(void)
	{
		DDRC = 0xff;		/* Порты для управления LCD-дисплеем */
		PORTC = 0x00;
		DDRD = 0x00;
		PORTD = 0xff;
//		DDRB = 1; // Порт PD0 как вывод
//		PORTB = 0x00;
		//	SFIOR = (SFIOR & !0x04);
		//	ACSR = 0x80; // Analog Comparator off
		//	ADCSRA = 0x00; // Analog-to-Digital Converter off
//		GIMSK = 0x00;
		TCCR0 = 0b00000001;	//Timer0 clk/1
		TCNT0 = 0x00;		//Timer0 start point
		TIMSK = (1 << TOIE0);	//Timer0 on
		MCUCR = 0b00000101;	/* Прерывание от INT0 и INT1 генерируется фронтом */
		GICR  = 0b11000000;
		sei(); //SREG = SREG | 0b10000000;
		LCD_init();
		WriteCommand(0b00001100);		// Включение дисплея без курсора, ничего не мигает
		string = (char *)calloc((D_SZ+1),sizeof(char));
		time_position = 0b10000000+1;
//		strcpy(string,"                ");
 		while(1)
		{	
//			WriteCommand(0b00000001);
//			strcpy(string,"NRNDDA          ");
//			WriteString(0b11000000,string);
//			_delay_ms(1000);
//			strcpy(string,"Progs           ");
//			WriteString(0b11000000,string);
//			_delay_ms(1000);
			My_disp();
		}
		free(string);
		return 0;
   	} 

