#include <avr/io.h>
   #include <util/delay.h>
   #define F_CPU 16000000UL  // 16 MHz

/*	int is_on(int num)
	{
		if (PINC == (PINC & (1<<num)))
		{
			return 1;
		}
		return 0;
	} */
   
   int main(void)              // начало основой программы
   {

	unsigned char B0Pressed = 0;  
	unsigned char B1Pressed = 0;  
	unsigned char B2Pressed = 0;  
	unsigned char B3Pressed = 0;  

	DDRD = 0xff;         
   DDRB = 0xff;		   
   DDRA = 0xff;		  
	DDRC = 0x00; 
	PORTC = 0xff;
			
        while (1) {              // Бесконечный цикл

//			PORTD = 0xff; /* установить "1" на всех линиях порта D */
//			PORTC = 0xff;
//			PORTB = 0xff;
//			_delay_ms(250);   // Ждем 0.25 сек.

//			PORTD = 0x00;  /* установить "0" на всех линиях порта D */
//			PORTB = 0x00;
			//PORTA = 0x00;
			//PORTB = 0x00;
			//PORTD = 0x00;
		//	PORTD = 0x00;
	//		PORTB = 0b00000001;
		//	PORTA = 0b00000001;
			PORTD = 0x00;
			PORTA = 0x00;
			PORTB = 0x00;

    //Обработка кнопки 0  
    if (B0Pressed == 1) //Если произошло нажатие на кнопку, 
    {                   // уведичивает PORTB, ждет отпускания
		PORTD = 0x00;
		PORTB = 0xff;
		PORTA = 0xff;		
      B0Pressed = 0;
      while ((PINC & (1 << PC0)) == 0)
      {}
    }
    else
    {
      if ((PINC & (1 << PC0)) == 0)      //Фиксирует нажатие
      {
        _delay_ms(5);        //Устранение "дребезга клавиш"
        if ((PINC & (1 << PC0)) == 0)    //Проверяет нажатие
        {
          B0Pressed = 1;  //Устанавливает флаг "кнопка нажата"
        }
      }    
    }


   //Обработка кнопки 1  
    if (B1Pressed == 1) //Если произошло нажатие на кнопку, 
    {                   // уведичивает PORTB, ждет отпускания
		PORTD = 0xff;
		PORTB = 0xff;
		PORTA = 0x00;		
      B1Pressed = 0;
      while ((PINC & (1 << PC1)) == 0)
      {}
    }
    else
    {
      if ((PINC & (1 << PC1)) == 0)      //Фиксирует нажатие
      {
        _delay_ms(5);        //Устранение "дребезга клавиш"
        if ((PINC & (1 << PC1)) == 0)    //Проверяет нажатие
        {
          B1Pressed = 1;  //Устанавливает флаг "кнопка нажата"
        }
      }    
    }

   //Обработка кнопки 2  
    if (B2Pressed == 1) //Если произошло нажатие на кнопку, 
    {                   // уведичивает PORTB, ждет отпускания
		PORTD = 0x00;
		PORTB = 0xff;
		PORTA = 0x00;		
      B2Pressed = 0;
      while ((PINC & (1 << PC2)) == 0)
      {}
    }
    else
    {
      if ((PINC & (1 << PC2)) == 0)      //Фиксирует нажатие
      {
        _delay_ms(5);        //Устранение "дребезга клавиш"
        if ((PINC & (1 << PC2)) == 0)    //Проверяет нажатие
        {
          B2Pressed = 1;  //Устанавливает флаг "кнопка нажата"
        }
      }    
    }

   //Обработка кнопки 3  
    if (B3Pressed == 1) //Если произошло нажатие на кнопку, 
    {                   // уведичивает PORTB, ждет отпускания
		PORTD = 0xff;
		PORTB = 0xff;
		PORTA = 0x00;
		_delay_ms(20);		
		PORTD = 0x00;
		PORTB = 0xff;
		PORTA = 0xff;
		_delay_ms(20);
		PORTD = 0x00;
		PORTB = 0xff;
		PORTA = 0x00;
		_delay_ms(20);
      B3Pressed = 0;
      while ((PINC & (1 << PC3)) == 0)
      {		
		PORTD = 0xff;
		PORTB = 0xff;
		PORTA = 0x00;
		_delay_ms(5);		
		PORTD = 0x00;
		PORTB = 0xff;
		PORTA = 0xff;
		_delay_ms(5);
		PORTD = 0x00;
		PORTB = 0xff;
		PORTA = 0x00;
		_delay_ms(5);
		}
    }
    else
    {
      if ((PINC & (1 << PC3)) == 0)      //Фиксирует нажатие
      {
        _delay_ms(5);        //Устранение "дребезга клавиш"
        if ((PINC & (1 << PC3)) == 0)    //Проверяет нажатие
        {
          B3Pressed = 1;  //Устанавливает флаг "кнопка нажата"
        }
      }    
    }
/*			

			PORTD = 0x00;
			PORTB = 0xff;
			PORTA = 0b00000001;

			_delay_ms(150);

			PORTA = 0b00000010;
			PORTB = 0xff;
			PORTD = 0b00000001;
					
			_delay_ms(150);

			PORTA = 0b00000101;
			PORTB = 0xff;
			PORTD = 0b00000010;
					
			_delay_ms(150);

			PORTA = 0b00001010;
			PORTB = 0xff;
			PORTD = 0b00000101;
					
			_delay_ms(150);

			PORTA = 0b00010101;
			PORTB = 0xff;
			PORTD = 0b00001010;
					
			_delay_ms(150);

			PORTA = 0b00101010;
			PORTB = 0xff;
			PORTD = 0b00010101;
					
			_delay_ms(150);

			PORTA = 0b01010101;
			PORTB = 0xff;
			PORTD = 0b00101010;
					
			_delay_ms(150);

			PORTA = 0b10101010;
			PORTB = 0xff;
			PORTD = 0b01010101;
					
			_delay_ms(150);

			while (1)
			{
				temp = PORTA;
				PORTA = PORTD;
				PORTD = temp;
			} */
		}       // закрывающая скобка бесконечного цикла
		return 0;
   }      // закрывающая скобка основной программы

