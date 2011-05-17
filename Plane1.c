#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL  // 16 MHz

	unsigned char min,sec,hour,t;
/*
  	int time(char a)
	{
		me=min%10;
		md=(min-min%10)/10;
		he=hour%10;
		hd=(hour-hour%10)/10;
		vivod(4,me,0);       
		_delay_ms(1);
		vivod(3,md,0);
		_delay_ms(1);
		vivod(2,he,a);       
		_delay_ms(1);
		vivod(1,hd,0);
		_delay_ms(1);   
		return 0;
	}
	 */
   
	SIGNAL (SIG_INTERRUPT0)
	{
		t+=1;
		if (t >= 128)
		{
			sec+=1;
			t=0;
		}
		if (sec >= 60) 
		{
			min+=1;
			sec=0;
		}
		if (min >= 60)
		{
			hour+=1;
			min=0;
		}
		if (hour >= 24)
		{
			hour = 0;
		}
	}

   int main(void)              // начало основой программы
   {
   	DDRC = 0xff;
   	DDRA = 0xff;
	PORTA = 0x00;	
//	TCCR0 = 0b000000111;	//Timer0 external source on rising edge
//	TCNT0 = 0x00;		//Timer0 start point
//	TIMSK = (1 << TOIE0);	//Timer0 on
//	MCUCR= 0x00;	// Idle 		// 0b10110000 - Power-save mode. Timer/Counter2 on
	MCUCR = 0b00000011;
	GICR = (1<<INT0);
	PORTA=0xff;
	sei();
        while (1)             // Бесконечный цикл
	{
		PORTC=0b00001111;
		if (sec!=0)
		{
			PORTC=0b11111111;
			sec=0;
			_delay_ms(100);
		}
	}
		return 0;
   }      // закрывающая скобка основной программы

