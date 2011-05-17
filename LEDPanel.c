#include <avr/io.h>
   #include <util/delay.h>
   #define F_CPU 8000000UL  // 8 MHz

   
   int main(void)              // начало основой программы
   {
   
   DDRD = 0xff;            /* все выводы порта D сконфигурировать как выходы */
   DDRB = 0x3f;		   /* 5 выводов порта B сконфигурировать как выходы */
   DDRC = 0x03;		   /* 2 вывода порта C сконфигурировать как выходы */
			
/*	int ein_1(void)
	{
		PORTD = 0b11110111;
		PORTB = 0b11011111;
		return 0;
	}

	int ein_2(void)
	{
		PORTD = 0b11111110;
		PORTB = 0b11111110;
		return 1;
	}

	int zwei_1(void)
	{
		PORTD = 0b11100011;
		PORTC = 0b11111100;
		return 1;
	}

	int zwei_2(void)
	{
		PORTD = 0b11111110;
		PORTB = 0b11111000;
		return 1;
	} */

        while (1) {              // Бесконечный цикл

//			PORTD = 0xff; /* установить "1" на всех линиях порта D */
//			PORTC = 0xff;
//			PORTB = 0xff;
//			_delay_ms(250);   // Ждем 0.25 сек.

//			PORTD = 0x00;  /* установить "0" на всех линиях порта D */
//			PORTC = 0x00;
//			PORTB = 0x00;

//			_delay_ms(250);
			PORTC = 0xff;
			PORTD = 0xff;
			PORTB = 0xff;
		
			_delay_ms(150);

			PORTD = 0b11111010;
			PORTB = 0b11111100; /* Рисуем 4 */

			_delay_ms(150);

			PORTD = 0b10010111;
			PORTB = 0b11011111; /* Рисуем 40 */

			_delay_ms(150);
			PORTD = 0b11111000;
			PORTB = 0b11110000; /* Рисуем 8 */

			_delay_ms(150);

			PORTD = 0b10000111;
			PORTB = 0b11011111; /* Рисуем 80 */
			PORTC = 0b11111100;

			_delay_ms(150);

			PORTD = 0b11111110;
			PORTB = 0b11111110; /* Рисуем 1 */
			PORTC = 0xff;

			_delay_ms(150);
			PORTD = 0b11110001;
			PORTB = 0b11011000; /* Рисуем 15 */

			_delay_ms(150);

			PORTD = 0b10001111;
			PORTB = 0b11011111; /* Рисуем 50 */
			PORTC = 0b11111110;


			_delay_ms(150);

			PORTD = 0b11111110;
			PORTB = 0b11111110; /* Рисуем 1 */
			PORTC = 0xff;

			_delay_ms(150);

			PORTD = 0b11110001;
			PORTB = 0b11010000; /* Рисуем 16 */

			_delay_ms(150);

			PORTD = 0b10001111;
			PORTB = 0b11011111; /* Рисуем 60 */
			PORTC = 0b11111100;

			_delay_ms(150);

			PORTD = 0b11111100;
			PORTB = 0b11110001; /* Рисуем 2 */
			PORTC = 0b11111111;

			_delay_ms(150);

			PORTD = 0b11000100;
			PORTC = 0x00;
			PORTB = 0b11111000; /* Рисуем 23 */

			_delay_ms(150);

			PORTD = 0b11000111;
			PORTB = 0b11011111; /* Рисуем 30 */
			PORTC = 0b11111110;

			_delay_ms(150);

			PORTD = 0b11111010;
			PORTB = 0b11111100; /* Рисуем 4 */
			PORTC = 0b11111111;

			_delay_ms(150);

			PORTD = 0b10010100;
			PORTC = 0xff;
			PORTB = 0b11010001; /* Рисуем 42 */
						
			_delay_ms(150);


			PORTD = 0b11000111;
			PORTB = 0b11111111; /* Рисуем 20 */
			PORTC = 0b11111100;

			_delay_ms(150);

//			PORTD = 0b10111000;
//			PORTC = 0x00;
//			PORTB = 0b11110010; /* Рисуем LO */

//			_delay_ms(250);

//			PORTD = 0b10001001;
//			PORTC = 0b11111110;
//			PORTB = 0b11010111; /* Рисуем ST */
//			_delay_ms(250);

		

		}       // закрывающая скобка бесконечного цикла
		return 0;
   }      // закрывающая скобка основной программы

