#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#ifndef LCD_RS_PORT
#define	LCD_RS_PORT 0
#endif

#ifndef LCD_RW_PORT
#define	LCD_RW_PORT 1
#endif

#ifndef LCD_E_PORT
#define	LCD_E_PORT  2
#endif

// LCD Data ports: PORTA[3:6]
#ifndef LCD_DATA_PORT
#define	LCD_DATA_PORT PORTA
#endif

#ifndef LCD_DATA_DDR
#define LCD_DATA_DDR DDRA
#endif

#ifndef LCD_DATA_PORT_SHIFT
#define	LCD_DATA_PORT_SHIFT 3
#endif

#define	LCD_DATA_TEMPLATE (0x0f << LCD_DATA_PORT_SHIFT)
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
#define SHIFT_DELAY 100

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
	_delay_ms(0.45 * LCD_TIME_MARGIN * TIME_SCALE); // Don't need delay between strobs but strobe cycle must be about 1ms alltogether
	LCD_MINOR_4_DATA_BITS_WRITE(data);
	strobe();

/*FOR READ: One dummy read is necessary right after the address setting.
  For details, refer to the explanation of output register in “Function of Each Block”.*/

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
    _delay_ms(0.043 * LCD_TIME_MARGIN * TIME_SCALE);
	return addr_temp;
}

void lcd_clear(void)
{ // Очищаем экран
	lcd_write(0x01,0);
    _delay_ms(1.52 * LCD_TIME_MARGIN * TIME_SCALE);//Slow operation
}

void lcd_return_home(void)
{ // Возвращает курсор в начало
	lcd_write(0x02,0);
    _delay_ms(1.52 * LCD_TIME_MARGIN * TIME_SCALE);//Slow operation
}

void lcd_goto(unsigned char address)
{ // Переводит указатель текущей ячейки памяти в lcd по новому адресу.
	lcd_write((0x80+address),0);
}

void lcd_shift(char mode, char direction)
{ // Shift display(mode='d') or cursor(mode='c') to the right(direction='r') or to the left(direction='l').
  // If parameters don't match ones above, then it shifts cursor to the right.
  lcd_write((0x10 + ((mode == 'd') << 3) + ((direction == 'l') << 2)),0);
}

void lcd_write_string(unsigned char start_position, unsigned char view_str_length, char * string)
{
	lcd_goto(start_position);//lcd_write(0x80 + start_position,0);
	unsigned char string_length = strlen(string);
//     unsigned char effective_length = string_length + start_position;
// 	unsigned char size = (view_str_length > LCD_X_SIZE) ? LCD_X_SIZE : view_str_length;
    unsigned char i;
    if (string_length == 0)
      return;
    else if (string_length <= LCD_X_SIZE)   //Visible range of RAM
      for (i=0;i<string_length;i++)
      {
          lcd_write(*(string + i),1);
      }
    else if (string_length <= 40)   //Invisible range of RAM
      for (i=0;i<string_length;i++)
      {
          lcd_write(*(string + i),1);
      }
    else
      for (i=0;i<40;i++)
      {
          lcd_write(*(string + i),1);
      }
// 	if (string_length < size)
// 		for (i = (size - string_length); i>0;i--)
// 		{
// 			lcd_write(0x20,1); //0x20 - ' '
// 		}
}

void lcd_running_string(char * string)
{
  lcd_goto(LCD_X_SIZE);//Jump to first invisible memory field
  unsigned char string_length = strlen(string);
  unsigned char splitting = (string_length > 24); //Do we need to split string?
  unsigned char size = splitting ? 24 : string_length; //There is only 24 bytes free.
  unsigned char i;
  for (i=0;i<size;i++)
  {
      lcd_write(*(string + i),1);
  }
  if (splitting == 1)
    do {
      lcd_shift('d','l');
      _delay_ms(SHIFT_DELAY * TIME_SCALE);
      lcd_goto(i%size);//Jump to last invisible memory field
      lcd_write(*(string + i),1);
      i++;
    }
    while (i < string_length);
  for (i=0;i<size;i++)
  {
    lcd_shift('d','l');
    _delay_ms(SHIFT_DELAY * TIME_SCALE);
  }
  for (i=0;i<(string_length%LCD_X_SIZE);i++)
  {
    lcd_shift('d','l');
    _delay_ms(SHIFT_DELAY * TIME_SCALE);
  }
}

void lcd_running_string_new(char * string)
{
  lcd_goto(LCD_X_SIZE);//Jump to first invisible memory field
  lcd_write(0x04+0x02+0x01,0); // Shift display to the right with cursor
  unsigned char string_length = strlen(string);
  unsigned char i;
  for (i=0;i<string_length;i++)
  {
    lcd_write(*(string + i),1);
    _delay_ms(SHIFT_DELAY * TIME_SCALE);
  }
  for (i=0;i<LCD_X_SIZE;i++)
  {
    lcd_write(' ',1);
    _delay_ms(SHIFT_DELAY * TIME_SCALE);
  }
  lcd_write(0x04+0x02+0x00,0); // Don't shift display to the right with cursor
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
		_delay_ms(0.4 * LCD_TIME_MARGIN * TIME_SCALE);//It was 1ms but 0.4ms will be enough
	}

	LCD_MINOR_4_DATA_BITS_WRITE(0b00000010);//0b00100000
	strobe();
	_delay_ms(0.4 * LCD_TIME_MARGIN * TIME_SCALE);//It was 1ms but 0.4ms will be enough

// 	BIT_SET(LCD_DATA_PORT,LCD_RW_PORT);
	lcd_port_state('z');

	lcd_write(0x28,0); //0b0010_1000 //Set bitness and lines number
	lcd_write(0x0C,0); // Включение дисплея без курсора, ничего не мигает
//     lcd_write(0x0F,0); // Включение дисплея без курсора, ничего не мигает
//     lcd_write(0x04+0x02+0x01,0); // Shift display to the right with cursor
	lcd_clear(); //lcd_write(0x01,0); // Очищаем экран
}
