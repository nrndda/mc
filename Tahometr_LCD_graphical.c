   #include <avr/io.h>   
   #include <util/delay.h>
//   #include <avr/interrupt.h>
   #include <stdlib.h>
   #include <string.h>
	#define LCD_CONTROL_PORT PORTA
	#define LCD_RS_PORT 0
	#define LCD_RW_PORT 1
	#define LCD_E_PORT  2
	#define LCD_CS1_PORT  3
	#define LCD_CS2_PORT  4
	#define LCD_RST_PORT  5
	#define LCD_DATA_PORT PORTC
	#define LCD_DATA_DDR DDRC
	
	#define LCD_RS(a)  LCD_CONTROL_PORT = a ? (LCD_CONTROL_PORT | (a<<LCD_RS_PORT)) : (LCD_CONTROL_PORT & ~((!a)<<LCD_RS_PORT))
	#define LCD_RW(a) LCD_CONTROL_PORT = a ? (LCD_CONTROL_PORT | (a<<LCD_RW_PORT)) : (LCD_CONTROL_PORT & ~((!a)<<LCD_RW_PORT))
	#define LCD_E(a)  LCD_CONTROL_PORT = a ? (LCD_CONTROL_PORT | (a<<LCD_E_PORT)) : (LCD_CONTROL_PORT & ~((!a)<<LCD_E_PORT))
	#define LCD_CS1(a) LCD_CONTROL_PORT = a ? (LCD_CONTROL_PORT | (a<<LCD_CS1_PORT)) : (LCD_CONTROL_PORT & ~((!a)<<LCD_CS1_PORT))
	#define LCD_CS2(a) LCD_CONTROL_PORT = a ? (LCD_CONTROL_PORT | (a<<LCD_CS2_PORT)) : (LCD_CONTROL_PORT & ~((!a)<<LCD_CS2_PORT))
	#define LCD_CS(a)  (a == 0) ?  (LCD_CS1(0),LCD_CS2(0)) : ((a == 1) ? (LCD_CS1(1),LCD_CS2(0)) : ((a == 2) ?  (LCD_CS1(0),LCD_CS2(1)) : (LCD_CS1(1),LCD_CS2(1))))
	#define LCD_X_SIZE 64				//Display's string size
	#define LCD_Y_SIZE 2*64				//Display's number of strings
	#define LCD_Z_STATE(a) (LCD_DATA_DDR = a ? 0x00 : 0xff,LCD_DATA_PORT = 0x00)
	
//	char sreg, max_speed = 0,EEPROM_max_speed,EEPROM_range;
//	char min = 0,hour = 0, sec = 0,flash=0,refresh = 4;
//	char light=0;
//	unsigned int time = 0;
//	double speed=0.0, turn=0.0, range=0.0;
	char * string;

	void LCD_WRITE(char DATA,char COMMAND_DATA, char CS_SELECT) //запись 1-го байта комманды по текущему адресу
	{
	  LCD_Z_STATE(0);
	  _delay_ms(5);
	  LCD_RW(0);
	  LCD_CS(CS_SELECT);
	  LCD_RS(COMMAND_DATA);
	  PORTC = DATA;
	  _delay_ms(5);
	  LCD_E(1);
	  _delay_ms(5);
	  LCD_E(0);
	  _delay_ms(1);
	  LCD_RW(1);
	  LCD_CS(0);
	  LCD_Z_STATE(1);
	}

/*	void LCD_WRITE_STRING(char POSITION, char * STRING)
	{
	  LCD_WRITE(POSITION,0);
	  for (int i=0;i<=LCD_X_SIZE;i++)
	    LCD_WRITE(*(STRING + i),1);
	}

	void LCD_init(void)
	{ 	  
	  LCD_WRITE(0b00111000,0);
	  _delay_ms(10);
	  LCD_WRITE(0b00001000,0);
	  _delay_ms(10);
	  LCD_WRITE(0b00000001,0);
	  _delay_ms(10);
	  LCD_WRITE(0b00000110,0);
	  _delay_ms(10);
	  }*/



	int main(void)
	{
		DDRA = 0xff;		/* Порты для управления LCD-дисплеем */
		PORTA = 0x00;
		
		DDRB = 0xff;
		PORTB = 0x00;
		
		DDRC = 0xff;
		PORTC = 0x00;
		
		DDRD = 0xff;
		PORTD = 0x00;
		
		DDRE = 0xff;
		PORTE = 0x00;
		
//		GIMSK = 0x00;
//		MCUCR = 0b00000001;	/* Прерывание от INT0 генерируется изменением напряжения, а на INT1 низким напряжением */
//		GICR  = 0b01000000;
//		sei();
		_delay_ms(50);
		LCD_WRITE(0x3F,0,3);	// Включение дисплея
		LCD_WRITE(0x40,0,3);
		LCD_WRITE(0xB8,0,3);
		LCD_WRITE(0xC0,0,3);
		string = (char *)malloc((LCD_X_SIZE+1)*sizeof(char));
//		time_position = 0b10000000+1;
 		for(;;)
		{	
			for (int i=0; i<256; i++)
			{
			  LCD_WRITE(i,0,1);
			  LCD_WRITE(i,0,2);
			  _delay_ms(100);
			}
		}
		free(string);
		return 0;
   	} 

