#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>

#define LCD_RS_PORT 0
#define LCD_RW_PORT 1
#define LCD_E_PORT  2
// LCD Data ports: PORTA[3:6]
#define LCD_DATA_PORT PORTA
#define LCD_DATA_PORT_SHIFT 3
#define LCD_DATA_DDR DDRA
//Display's string size
#define LCD_X_SIZE 12
//Display's number of strings
#define LCD_Y_SIZE 2
#define LCD_Z_STATE(a)	LCD_DATA_DDR = (a) ? (LCD_DATA_DDR & 0x80) : (LCD_DATA_DDR | 0x7f);\
								LCD_DATA_PORT = LCD_DATA_PORT & 0x80
#define LCD_READ(a)		LCD_DATA_DDR = (a) ? (LCD_DATA_DDR & 0x87) : (LCD_DATA_DDR | 0x7f);\
								LCD_DATA_PORT = LCD_DATA_PORT & 0x80
#define LCD_DATA_WRITE(a) LCD_DATA_PORT = (LCD_DATA_PORT & 0x87) | (a)
#define BIT_INVERSE(value,bit) ((value) ^= (1 << (bit)))
#define BIT_SET(value,bit) ((value) |= (1 << (bit)))
#define BIT_CLEAR(value,bit) ((value) &= (~(1 << (bit))))
#define IS_BIT_SET(value,bit) ((((value) & (1 << (bit))) != 0) ? 1 : 0)
// TIME_SCALE=1 - минимальное значение.
#define TIME_SCALE 1

void strobe(void);
unsigned char lcd_write(unsigned char, unsigned char);
void lcd_write_string(unsigned char, unsigned char, char *);
void lcd_init(void);
unsigned char USART_Receive(void);
void USART_Transmit(unsigned char);
int fpower(int, int);

int main(void)
{
	LCD_DATA_DDR = 0xff;/* Порты для управления LCD-дисплеем. Сначала как вход*/
//	BIT_SET(DDRA,7); // Светодиод
	PORTA = 0x00;

	DDRB = 0xff;
	PORTB = 0x00;

	DDRC = 0xff;
	PORTC = 0x00;

	DDRD = 0xff;
	PORTD = 0x00;

	DDRE = 0xff;
	PORTE = 0x00;

	UBRRH = 0x06;	// Делитель частоты
	UBRRL = 0x07;	// для USART. 0x67 = 103 для 9600bps
//	UCSRA = (0<<U2X);
	UCSRB = (1<<RXEN)|(1<<TXEN);
	UCSRC = (1<<URSEL)|(0<<USBS)|(3<<UCSZ0);
	
//	BIT_SET(PORTA,7);
//	_delay_ms(100 * TIME_SCALE);
//	BIT_CLEAR(PORTA,7);
	//		GIMSK = 0x00;
	//		MCUCR = 0b00000001;	/* Прерывание от INT0 генерируется изменением напряжения, а на INT1 низким напряжением */
	//		GICR  = 0b01000000;
	//		sei();
//	_delay_ms(10 * TIME_SCALE);
	lcd_init();
//	BIT_INVERSE(PORTA,7);
//	_delay_ms(100 * TIME_SCALE);
	lcd_write(0b00001100,0);	// Включение дисплея без курсора, ничего не мигает
	lcd_write(0x01,0); // Очищаем экран
//	char * string = (char *)malloc((4*LCD_X_SIZE+1)*sizeof(char));
//	char * string_backup = string;
//	char temp;
	for (;;)
	{	
/*		string = string_backup;
//		BIT_INVERSE(PORTA,7);
		lcd_write(0x01,0);
		lcd_write(0x80,0);
		_delay_ms(100 * TIME_SCALE);
		//
		for (unsigned char i=0;i<LCD_X_SIZE;i++)
		{
			lcd_write(0x30 + i,1);
			_delay_ms(50 * TIME_SCALE);
		}
//		lcd_write(0xC0,0); //0xC0 = 0x80 + 0x40
		strcpy(string,"Test string! Maded by NRNDDA;)");
		lcd_write((0x80+0x40),0);
		lcd_write(0x85,1);
		lcd_write((0x80+LCD_X_SIZE-2),0);
		lcd_write(0x84,1);
		do
		{
			lcd_write_string(0x41,LCD_X_SIZE-2,string);
			_delay_ms(50 * TIME_SCALE);
			string++;
		}
		while (strlen(string) > 0);
		_delay_ms(50 * TIME_SCALE);*/
		unsigned char USART_Data = USART_Receive();
		lcd_write(USART_Data,1);
		unsigned char addr = lcd_write(USART_Data,1);
		if ((0x0C < addr) && (addr < 0x40)) lcd_write(0x40,0);
		if (0x4C < addr) lcd_write(0x00,0);
		_delay_ms(10 * TIME_SCALE);
	}
//	free(string);
	return 0;
}

void strobe(void)
{
	_delay_ms(0.07 * TIME_SCALE);
	BIT_SET(LCD_DATA_PORT,LCD_E_PORT);
	_delay_ms(0.5 * TIME_SCALE);
	BIT_CLEAR(LCD_DATA_PORT,LCD_E_PORT);
	_delay_ms(0.03 * TIME_SCALE);
}

unsigned char lcd_write(unsigned char data, unsigned char command_or_data) //запись 1-го байта комманды по текущему адресу. Возвращает адрес курсора.
{
	LCD_Z_STATE(0);
	if (command_or_data) BIT_SET(LCD_DATA_PORT,LCD_RS_PORT);
	else BIT_CLEAR(LCD_DATA_PORT,LCD_RS_PORT);
	BIT_CLEAR(LCD_DATA_PORT,LCD_RW_PORT);
	_delay_ms(0.6 * TIME_SCALE);
	LCD_DATA_WRITE((data & 0xf0) >> 1);
	strobe();
	_delay_ms(0.7 * TIME_SCALE);
	LCD_DATA_WRITE((data & 0x0f) << 3);
	strobe();
	
	LCD_READ(1);
	BIT_SET(LCD_DATA_PORT,LCD_RW_PORT);
	BIT_CLEAR(LCD_DATA_PORT,LCD_RS_PORT);
	_delay_ms(0.07 * TIME_SCALE);
	BIT_SET(LCD_DATA_PORT,LCD_E_PORT);
	_delay_ms(0.5 * TIME_SCALE);
	unsigned char busy = IS_BIT_SET(LCD_DATA_PORT,6); // DB7
	unsigned char addr_temp = LCD_DATA_PORT;
	BIT_CLEAR(LCD_DATA_PORT,LCD_E_PORT);
	_delay_ms(0.03 * TIME_SCALE);
	
	_delay_ms(0.07 * TIME_SCALE);
	BIT_SET(LCD_DATA_PORT,LCD_E_PORT);
	_delay_ms(0.5 * TIME_SCALE);
	addr_temp = ((addr_temp & 011110111) << 4) | LCD_DATA_PORT; // & убираем Busy flag из адреса.
	BIT_CLEAR(LCD_DATA_PORT,LCD_E_PORT);
	_delay_ms(0.03 * TIME_SCALE);
	if (busy) _delay_ms(0.1 * TIME_SCALE);
	LCD_Z_STATE(1);
	return addr_temp;
}

void lcd_write_string(unsigned char start_position, unsigned char view_str_length, char * string)
{
	lcd_write(0x80 + start_position,0);
	unsigned char temp = strlen(string);
	unsigned char size = (view_str_length > LCD_X_SIZE) ? LCD_X_SIZE : view_str_length;
	for (unsigned char i=0;i<size;i++)
	{
		lcd_write(*(string + i),1);
	}
	if (temp < size)
		for (unsigned char i = (size - temp); i>0;i--)
		{
			lcd_write(0x20,1); //0x20 - ' '
		}
}

void lcd_init(void)
{
	// Закоментированное только для 8-битного режима передачи
	//	  _delay_ms(40);
	//	  lcd_write(0x30,0);
	//	  _delay_ms(20);
	//	  lcd_write(0x30,0);
	//	  _delay_ms(20);
	//	  lcd_write(0x30,0);
	//	  _delay_ms(20);
	//	  lcd_write(0x38,0);
	_delay_ms(40 * TIME_SCALE);
	LCD_Z_STATE(0);
	BIT_CLEAR(LCD_DATA_PORT,LCD_RS_PORT);
	BIT_CLEAR(LCD_DATA_PORT,LCD_RW_PORT);
	
	LCD_DATA_WRITE(0b00011000);
	for (unsigned char i=0;i<3;i++)
	{
		strobe();
		_delay_ms(0.06 * TIME_SCALE);
	}
	
	LCD_DATA_WRITE(0b00010000);
	strobe();
	
	BIT_SET(LCD_DATA_PORT,LCD_RW_PORT);
	LCD_Z_STATE(1);
	
	lcd_write(0x28,0); //0b0010_1000
	lcd_write(0x08,0);
	lcd_write(0x01,0);
	lcd_write(0x06,0);
}

unsigned char USART_Receive(void)
{
	/* Wait for data to be received */
	while ( !(UCSRA & (1<<RXC)) )
		;
	/* Get and return received data from buffer */
	return UDR;
}

void USART_Transmit(unsigned char data)
{
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)) )
		;
	/* Put data into buffer, sends the data */
	UDR = data;
}

int fpower(int number,int power)
{
	int temp = number;
	if (power == 0) return 1;
	for (int n=2;n<=power;n++)
		temp = temp * number;
	return temp;
}
