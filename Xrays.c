#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL  // 16 MHz

	unsigned long int tick=0,tick1=0,sec=0;
	SIGNAL (SIG_OVERFLOW0)
	{
		tick++;
	}

   int main(void)              // начало основой программы
   {
   	DDRC = 0xff;
	PORTC= 0x00;
   	DDRB = 0xff;
	PORTB = 0x00;	

	TCCR0 = 0b00000001;	//Timer0 internal source / 1024
	TCNT0 = 0x00;		//Timer0 start point
	TIMSK = 0b00000010;	//Timer0 on
//	MCUCR= 0x00;	// Idle 		// 0b10110000 - Power-save mode. Timer/Counter2 on
//	MCUCR = 0b00000011;
//	GICR = (1<<INT0);
	tick=0;
	sei();
    while (1)             // Бесконечный цикл
	{
	tick++;
		if (tick%16000==0)
		{
			tick1++;
			tick=0;
			
		}
		if(tick1%1000==0)
		{
			sec++;
			tick1=0;
		}
		if(sec>60)
		{
			PORTB= 0xff;
		}
	}
		return 0;
   }      // закрывающая скобка основной программы

