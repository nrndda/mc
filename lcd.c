#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#define	LCD_RS_PORT 0
#define	LCD_RW_PORT 1
#define	LCD_E_PORT  2
// LCD Data ports: PORTA[3:6]
#define	LCD_DATA_PORT PORTA
#define	LCD_DATA_PORT_SHIFT 3
#define	LCD_DATA_TEMPLATE (0x0f << LCD_DATA_PORT_SHIFT)
#define	LCD_DATA_DDR DDRA
//Display's string size
#define	LCD_X_SIZE 12
//Display's number of strings
#define	LCD_Y_SIZE 2
// First four bits
#define LCD_MINOR_4_DATA_BITS_WRITE(data) LCD_DATA_PORT = ((LCD_DATA_PORT & (~LCD_DATA_TEMPLATE)) | ((0x0f & data) << LCD_DATA_PORT_SHIFT));

#define BIT_INVERSE(value,bit) ((value) ^= (1 << (bit)))
#define BIT_SET(value,bit) ((value) |= (1 << (bit)))
#define BIT_CLEAR(value,bit) ((value) &= (~(1 << (bit))))
#define IS_BIT_SET(value,bit) ((((value) & (1 << (bit))) != 0) ? 1 : 0)

//Запас по времени на задержки 20%
#define LCD_TIME_MARGIN 1.2

void strobe(void);
unsigned char lcd_write(unsigned char, unsigned char);
void lcd_clear(void);
void lcd_return_home(void);
void lcd_goto(unsigned char);
void lcd_write_string(unsigned char, unsigned char, char *);
void lcd_init(void);

void strobe(void)
{
	_delay_ms(0.06 * LCD_TIME_MARGIN * TIME_SCALE);
	BIT_SET(LCD_DATA_PORT,LCD_E_PORT);
	_delay_ms(0.45 * LCD_TIME_MARGIN * TIME_SCALE);
	BIT_CLEAR(LCD_DATA_PORT,LCD_E_PORT);
	_delay_ms(0.02 * LCD_TIME_MARGIN * TIME_SCALE);
}

void lcd_port_state(char state)//Режим работы портов lcd
{
	int ddr_temp = LCD_DATA_TEMPLATE | (0x01 << LCD_RS_PORT) |
				(0x01 << LCD_RW_PORT) | (0x01 << LCD_E_PORT);// На все управляющие пины по 1-це
	switch (state)
	{
		case 'o':
		case 'O':
		case '0':
		{
			LCD_DATA_DDR = (LCD_DATA_DDR | ddr_temp);
			LCD_DATA_PORT = LCD_DATA_PORT & 0x80;
		}; break;

		case 'i':
		case 'I':
		case '1':
		{
			LCD_DATA_DDR = (LCD_DATA_DDR & (~ddr_temp));
			LCD_DATA_PORT = LCD_DATA_PORT | 0x7f;
		}; break;

		case 'z':
		case 'Z':
		case '2':
		default:
		{
			LCD_DATA_DDR = (LCD_DATA_DDR & (~ddr_temp));
			LCD_DATA_PORT = LCD_DATA_PORT & 0x80;
		}; break;
	}

}

unsigned char lcd_write(unsigned char data, unsigned char command_or_data) //запись 1-го байта комманды по текущему адресу. Возвращает адрес курсора.
{
	lcd_port_state('o');
	if (command_or_data) BIT_SET(LCD_DATA_PORT,LCD_RS_PORT);
	else BIT_CLEAR(LCD_DATA_PORT,LCD_RS_PORT);
	BIT_CLEAR(LCD_DATA_PORT,LCD_RW_PORT);

	LCD_MINOR_4_DATA_BITS_WRITE(data >> 4);
	strobe();
	_delay_ms(1 * LCD_TIME_MARGIN * TIME_SCALE);
	LCD_MINOR_4_DATA_BITS_WRITE(data);
	strobe();

	unsigned char addr_temp = 0;
/*	lcd_port_state('i');
	BIT_SET(LCD_DATA_PORT,LCD_RW_PORT);
	BIT_CLEAR(LCD_DATA_PORT,LCD_RS_PORT);
	_delay_ms(0.07 * TIME_SCALE);
	BIT_SET(LCD_DATA_PORT,LCD_E_PORT);
	_delay_ms(0.5 * TIME_SCALE);
	unsigned char busy = IS_BIT_SET(LCD_DATA_PORT,6); // DB7
	addr_temp = LCD_DATA_PORT;
	BIT_CLEAR(LCD_DATA_PORT,LCD_E_PORT);
	_delay_ms(0.03 * TIME_SCALE);

	_delay_ms(0.07 * TIME_SCALE);
	BIT_SET(LCD_DATA_PORT,LCD_E_PORT);
	_delay_ms(0.5 * TIME_SCALE);
	addr_temp = ((addr_temp & 011110111) << 4) | LCD_DATA_PORT; // & убираем Busy flag из адреса.
	BIT_CLEAR(LCD_DATA_PORT,LCD_E_PORT);
	_delay_ms(0.03 * TIME_SCALE);
	if (busy) _delay_ms(0.1 * TIME_SCALE);*/
	lcd_port_state('z');
	return addr_temp;
}

void lcd_clear(void)
{ // Очищаем экран
	lcd_write(0x01,0);
}

void lcd_return_home(void)
{ // Возвращает курсор в начало
	lcd_write(0x02,0);
}

void lcd_goto(unsigned char address)
{ // Переводит указатель текущей ячейки памяти в lcd по новому адресу.
	lcd_write((0x80+address),0);
}

void lcd_write_string(unsigned char start_position, unsigned char view_str_length, char * string)
{
	lcd_goto(start_position);//lcd_write(0x80 + start_position,0);
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
	LCD_DATA_DDR = 0x00;/* Порты для управления LCD-дисплеем. Сначала как tri-sate*/
	LCD_DATA_PORT = 0x00;

	// Закоментированное только для 8-битного режима передачи
	//	  _delay_ms(40);
	//	  lcd_write(0x30,0);
	//	  _delay_ms(20);
	//	  lcd_write(0x30,0);
	//	  _delay_ms(20);
	//	  lcd_write(0x30,0);
	//	  _delay_ms(20);
	//	  lcd_write(0x38,0);
	_delay_ms(20 * LCD_TIME_MARGIN * TIME_SCALE);
	lcd_port_state('o');
	BIT_CLEAR(LCD_DATA_PORT,LCD_RS_PORT);
	BIT_CLEAR(LCD_DATA_PORT,LCD_RW_PORT);
	BIT_CLEAR(LCD_DATA_PORT,LCD_E_PORT);

	LCD_MINOR_4_DATA_BITS_WRITE(0b00000011);// Передаю только младшие четыре байта, поэтому число стоит не 0b00011000, а 0b00000011
	for (unsigned char i=0;i<3;i++)
	{
		strobe();
		_delay_ms(1 * LCD_TIME_MARGIN * TIME_SCALE);
	}

	LCD_MINOR_4_DATA_BITS_WRITE(0b00000010);//0b00100000
	strobe();
	_delay_ms(1 * LCD_TIME_MARGIN * TIME_SCALE);

// 	BIT_SET(LCD_DATA_PORT,LCD_RW_PORT);
	lcd_port_state('z');

	lcd_write(0x28,0); //0b0010_1000
	lcd_write(0x0C,0); // Включение дисплея без курсора, ничего не мигает
	lcd_write(0x01,0); // Очищаем экран
}