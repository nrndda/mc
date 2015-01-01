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
#define LED_FLASH_TIME 400
#define STRING_CHARSNUM 50

#define LCD_DATA_PORT PORTA
#define LCD_DATA_DDR DDRA
#include <lcd.c>
#include <led_strip.c>

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

// unsigned char led_array [432] = { };
unsigned char my_cos [] = {31,31,31,31,31,31,31,31,30,30,30,30,30,29,29,29,29,28,28,
                        28,27,27,27,26,26,25,25,24,24,23,23,22,22,21,21,20,20,19,
                        18,18,17,17,16,15,15,14,13,13,12,11,10,10,9,8,8,7,6,5,5,
                        4,3,2,2,1,0,-1,-2,-2,-3,-4,-5,-5,-6,-7,-8,-8,-9,-10,-10,
                        -11,-12,-13,-13,-14,-15,-15,-16,-17,-17,-18,-18,-19,-20,
                        -20,-21,-21,-22,-22,-23,-23,-24,-24,-25,-25,-26,-26,-27,
                        -27,-27,-28,-28,-28,-29,-29,-29,-29,-30,-30,-30,-30,-30,
                        -31,-31,-31,-31,-31,-31,-31,-31,-31,-31,-31,-31,-31,-31,
                        -31,-30,-30,-30,-30,-30,-29,-29,-29,-29,-28,-28,-28,-27,
                        -27,-27,-26,-26,-25,-25,-24,-24,-23,-23,-22,-22,-21,-21,
                        -20,-20,-19,-18,-18,-17,-17,-16,-15,-15,-14,-13,-13,-12,
                        -11,-10,-10,-9,-8,-8,-7,-6,-5,-5,-4,-3,-2,-2,-1,0,1,2,2,
                        3,4,5,5,6,7,8,8,9,10,10,11,12,13,13,14,15,15,16,17,17,18,
                        18,19,20,20,21,21,22,22,23,23,24,24,25,25,26,26,27,27,27,
                        28,28,28,29,29,29,29,30,30,30,30,30,31,31,31,31,31,31,31};
unsigned char my_mod_cos [] = {31,31,31,31,31,31,31,31,30,30,30,30,30,29,29,29,29,28,28,
                        28,27,27,27,26,26,25,25,24,24,23,23,22,22,21,21,20,20,19,
                        18,18,17,17,16,15,15,14,13,13,12,11,10,10,9,8,8,7,6,5,5,
                        4,3,2,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,2,
                        3,4,5,5,6,7,8,8,9,10,10,11,12,13,13,14,15,15,16,17,17,18,
                        18,19,20,20,21,21,22,22,23,23,24,24,25,25,26,26,27,27,27,
                        28,28,28,29,29,29,29,30,30,30,30,30,31,31,31,31,31,31,31};

void led_flash(char color)
{
  unsigned char led_pin;
  if (color == 'r')
    led_pin = 0;
  else if (color == 'g')
    led_pin = 2;
  else
    return;
  BIT_SET(PORTB,led_pin);
  _delay_ms(LED_FLASH_TIME * TIME_SCALE);
  BIT_CLEAR(PORTB,led_pin);
}

int main(void)
{
    LCD_DATA_DDR = 0x00;/* Порты для управления LCD-дисплеем. Сначала как tri-sate*/
    LCD_DATA_PORT = 0x00;

	DDRB = 0x00;//tri-sate
	PORTB = 0x00;
	BIT_SET(DDRB,0); // Светодиод
	BIT_SET(DDRB,1); // Светодиод
	BIT_SET(DDRB,2); // Светодиод
    led_strip_init();

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
    _delay_ms(1000 * TIME_SCALE);
    led_flash('g');
//     lcd_init();
    _delay_ms(1000 * TIME_SCALE);
    led_flash('g');

//     char * string = (char *)malloc((STRING_CHARSNUM+1)*sizeof(char));
    unsigned char step,led;
//     int j,i;
    for (;;)
    {
//       led_flash('r');
//       _delay_ms(1 * TIME_SCALE);
      for (step=255;step>=0;step--)
      {
        for (led=0;led<255;led++)
          set_led(my_mod_cos[(unsigned char)(led+step)],my_mod_cos[(unsigned char)(led+step+120/*65*/)],my_mod_cos[(unsigned char)(led+step+185/*127*/)]);
        led_strip_reset();
//         _delay_ms(1 * TIME_SCALE);
      }
  //         set_led((i<72) ? 255-3.5*i : 0,255-(i/4.5-15.97)*(i/4.5-15.97),(i>72) ? 3.5*i : 0);
//         _delay_ms(500 * TIME_SCALE);

//       lcd_clear();
//       lcd_write(0x21,1);
//       _delay_ms(1000 * TIME_SCALE);
//       lcd_clear();
//       for (i=0;i<40;i++)
//         lcd_write(0x30+i,1);
//       for (i=0;i<40;i++)
//       {
//         _delay_ms(SHIFT_DELAY * TIME_SCALE);
//         lcd_shift('d','r');
//       }
//       _delay_ms(1000 * TIME_SCALE);
//       led_flash('g');
//       lcd_clear();
//       _delay_ms(100 * TIME_SCALE);
//
//       for (i=0;i<40;i++)
//         lcd_write(0x30+i,1);
//       for (i=0;i<40;i++)
//       {
//         _delay_ms(SHIFT_DELAY * TIME_SCALE);
//         lcd_shift('d','l');
//       }
//       _delay_ms(1000 * TIME_SCALE);
//       led_flash('g');

//       strcpy(string,"Test string! Made by NRNDDA;)");
//       lcd_running_string(string);
//       lcd_goto(0x40);//lcd_write((0x80+0x40),0);
//       lcd_write(0x85,1);
//       lcd_goto(LCD_X_SIZE-2);//lcd_write((0x80+LCD_X_SIZE-2),0);
//       lcd_write(0x84,1);
//       do
//         {
//           lcd_write_string(0x41,LCD_X_SIZE-2,string);
//           _delay_ms(50 * TIME_SCALE);
//           string++;
//         }
//       while (strlen(string) > 0);
      /*_delay_ms(50 * TIME_SCALE);
      unsigned char USART_Data = USART_Receive();
      lcd_write(USART_Data,1);
      unsigned char addr = lcd_write(USART_Data,1);
      if ((0x0C < addr) && (addr < 0x40)) lcd_write(0x40,0);
      if (0x4C < addr) lcd_write(0x00,0);
      _delay_ms(10 * TIME_SCALE);*/
    }
//     free(string);
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
