   #include <avr/io.h>
   #include <util/delay.h>
   #include <avr/interrupt.h>
   #include <stdlib.h>
   #include <string.h>
	#define LCD_RS_PORT 0
	#define LCD_RW_PORT 1
	#define LCD_E_PORT  2
	#define LCD_DATA_PORT PORTC
	#define LCD_DATA_DDR DDRC

	#define LCD_RS(a)  PORTA = a ? (PORTA | (a<<LCD_RS_PORT)) : (PORTA & ~((!a)<<LCD_RS_PORT))
	#define LCD_RW(a) PORTA = a ? (PORTA | (a<<LCD_RW_PORT)) : (PORTA & ~((!a)<<LCD_RW_PORT))
	#define LCD_E(a)  PORTA = a ? (PORTA | (a<<LCD_E_PORT)) : (PORTA & ~((!a)<<LCD_E_PORT))
	#define LCD_X_SIZE 16				//Display's string size
	#define LCD_Y_SIZE 2				//Display's number of strings
	#define LCD_Z_STATE(a) (LCD_DATA_DDR = a ? 0x00 : 0xff,LCD_DATA_PORT = 0x00)

//	char sreg, max_speed = 0,EEPROM_max_speed,EEPROM_range;
//	char min = 0,hour = 0, sec = 0,flash=0,refresh = 4;
//	char light=0;
//	unsigned int time = 0;
//	double speed=0.0, turn=0.0, range=0.0;
	char * string;

	void LCD_WRITE(char DATA,char COMMAND_DATA) //запись 1-го байта комманды по текущему адресу
	{
	  LCD_Z_STATE(0);
	  LCD_RS(COMMAND_DATA);
	  LCD_RW(0);
	  _delay_ms(5);
	  LCD_DATA_PORT = DATA;
	  _delay_ms(5);
	  LCD_E(1);
	  _delay_ms(10);
	  LCD_E(0);
	  _delay_ms(10);
	  LCD_RW(1);
	  _delay_ms(5);
	  LCD_Z_STATE(1);
	  PORTE = (PORTE & (1<<1)) ? (PORTE & ~(1<<1)) : (PORTE | (1<<1));
	}

	void LCD_WRITE_STRING(char POSITION, char * STRING)
	{
	  LCD_WRITE(POSITION,0);
	  for (int i=0;i<LCD_X_SIZE;i++)
	  {
	    LCD_WRITE(*(STRING + i),1);
	  }
	}

	void LCD_INIT(void)
	{
	  _delay_ms(40);
	  LCD_WRITE(0x30,0);
	  _delay_ms(20);
	  LCD_WRITE(0x30,0);
	  _delay_ms(20);
	  LCD_WRITE(0x30,0);
	  _delay_ms(20);
	  LCD_WRITE(0x38,0);
	  LCD_WRITE(0x08,0);
	  LCD_WRITE(0x01,0);
	  LCD_WRITE(0x06,0);
	  _delay_ms(10);
	  PORTE = 0x01;
	}



	int main(void)
	{
		DDRA = 0xff;
		PORTA = 0x00;

		DDRB = 0xff;
		PORTB = 0x00;

		DDRC = 0x00;		/* Порты для управления LCD-дисплеем. Сначала высокий импеданс!*/
		PORTC = 0x00;

		DDRD = 0xff;
		PORTD = 0x00;

		DDRE = 0xff;
		PORTE = 0x00;

//		GIMSK = 0x00;
//		MCUCR = 0b00000001;	/* Прерывание от INT0 генерируется изменением напряжения, а на INT1 низким напряжением */
//		GICR  = 0b01000000;
//		sei();
		LCD_INIT();
		_delay_ms(10);
		LCD_WRITE(0b00001100,0);	// Включение дисплея без курсора, ничего не мигает
		string = (char *)malloc((LCD_X_SIZE+1)*sizeof(char));
//		time_position = 0b10000000+1;
		char temp;
 		for (;;)
		{
			LCD_WRITE(0x01,0);
			PORTE = (PORTE & (1<<2)) ? (PORTE & ~(1<<2)) : (PORTE | (1<<2));
//			LCD_WRITE(0x40,0);
			for (temp='A';temp<='Z';temp++)
			{
			  LCD_WRITE(temp,1);
			  _delay_ms(100);
			}
			LCD_WRITE(0xC0,0);
			for (temp='A';temp<='Z';temp++)
			{
			  LCD_WRITE(temp,1);
			  _delay_ms(100);
			}
			LCD_WRITE(0x93,1);
			LCD_WRITE(0x3B,1);
			_delay_ms(1000);
			LCD_WRITE(0x01,0);
			PORTE = (PORTE & (1<<2)) ? (PORTE & ~(1<<2)) : (PORTE | (1<<2));
			strcpy(string,"nrndda");
			LCD_WRITE_STRING(0b11000000,string);
			_delay_ms(1000);

		}
		free(string);
		return 0;
   	}

