#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>

#define BIT_INVERSE(value,bit) ((value) ^= (1 << (bit)))
#define BIT_SET(value,bit) ((value) |= (1 << (bit)))
#define BIT_CLEAR(value,bit) ((value) &= (~(1 << (bit))))
#define IS_BIT_SET(value,bit) ((((value) & (1 << (bit))) != 0) ? 1 : 0)
// TIME_SCALE=1 - минимальное значение.
#define TIME_SCALE 1

#include <lcd.c>

void strobe(void);
unsigned char lcd_write(unsigned char, unsigned char);
void lcd_clear(void);
void lcd_return_home(void);
void lcd_goto(unsigned char);
void lcd_write_string(unsigned char, unsigned char, char *);
void lcd_init(void);
unsigned char USART_Receive(void);
void USART_Transmit(unsigned char);
int fpower(int, int);

int main(void)
{
	LCD_DATA_DDR = 0x00;/* Порты для управления LCD-дисплеем. Сначала как tri-sate*/
//	BIT_SET(LCD_DATA_DDR,7); // Светодиод
	LCD_DATA_PORT = 0x00;

	DDRB = 0x00;//tri-sate
	PORTB = 0x00;
	BIT_SET(DDRB,0); // Светодиод
	BIT_SET(DDRB,1); // Светодиод
	BIT_SET(DDRB,2); // Светодиод
	BIT_SET(PORTB,0);

	DDRC = 0x00;//tri-sate
	PORTC = 0x00;

	DDRD = 0x00;//tri-sate
	PORTD = 0x00;

	DDRE = 0x00;//tri-sate
	PORTE = 0x00;

//	UBRRH = 0x06;	// Делитель частоты
//	UBRRL = 0x07;	// для USART. 0x67 = 103 для 9600bps
//	UCSRA = (0<<U2X);
//	UCSRB = (1<<RXEN)|(1<<TXEN);
//	UCSRC = (1<<URSEL)|(0<<USBS)|(3<<UCSZ0);

// 	GIMSK = 0x00;
// 	MCUCR = 0b00000001;	/* Прерывание от INT0 генерируется изменением напряжения, а на INT1 низким напряжением */
// 	GICR  = 0b01000000;
// 	sei();
	_delay_ms(10 * TIME_SCALE);
	lcd_init();
	BIT_SET(PORTB,2);
	_delay_ms(100 * TIME_SCALE);
	char * string = (char *)malloc((4*LCD_X_SIZE+1)*sizeof(char));
	char * string_backup = (char *)malloc((4*LCD_X_SIZE+1)*sizeof(char));
	//	char temp;
	BIT_CLEAR(PORTB,0);
	for (;;)
	{
		*string = *string_backup;
		lcd_clear();
		lcd_return_home();
		_delay_ms(100 * TIME_SCALE);
		//
		for (unsigned char i=0;i<LCD_X_SIZE;i++)
		{
			lcd_write(0x30 + i,1);
			_delay_ms(50 * TIME_SCALE);
		}
//		lcd_write(0xC0,0); //0xC0 = 0x80 + 0x40
		strcpy(string,"Test string! Maded by NRNDDA;)");
		lcd_goto(0x40);//lcd_write((0x80+0x40),0);
		lcd_write(0x85,1);
		lcd_goto(LCD_X_SIZE-2);//lcd_write((0x80+LCD_X_SIZE-2),0);
		lcd_write(0x84,1);
		do
		{
			lcd_write_string(0x41,LCD_X_SIZE-2,string);
			_delay_ms(50 * TIME_SCALE);
			string++;
		}
		while (strlen(string) > 0);
		_delay_ms(50 * TIME_SCALE);
		unsigned char USART_Data = USART_Receive();
		lcd_write(USART_Data,1);
		unsigned char addr = lcd_write(USART_Data,1);
		if ((0x0C < addr) && (addr < 0x40)) lcd_write(0x40,0);
		if (0x4C < addr) lcd_write(0x00,0);
		_delay_ms(10 * TIME_SCALE);
	}
	free(string);
	free(string_backup);
	return 0;
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
