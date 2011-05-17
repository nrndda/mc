#define RUN
#ifdef RUN
#include "avr/io.h"
#include <util/delay.h>
#include <avr/interrupt.h> 

#else
#include <stdio.h>
#include <dos.h>
char PORTB, DDRB,DDRC,DDRD;
#define F_CPU 4000000UL  // 4 MHz
#endif


const int DOT_LEN = 100;
const int SLASH_LEN= 300;
const int CHR_SPACE_LEN= 100; 
const int SPACE_LEN= 200;
const int PAUSE= 3000;

typedef struct 
{
	int seq[5];
} code;

code Morze[32];

	void convert(void)
	{
		int i,j;
		for(i=0;i<32;i++)
		{
			for(j=0;j<5;j++)
				Morze[i].seq[j]=0;
		}

		// A = ._
		Morze[0].seq[0]=1;
		Morze[0].seq[1]=2;

		// B = _...
		Morze[1].seq[0]=2;
		Morze[1].seq[1]=1;
		Morze[1].seq[2]=1;
		Morze[1].seq[3]=1;

		// C = _._.
		Morze[2].seq[0]=2;
		Morze[2].seq[1]=1;
		Morze[2].seq[2]=2;
		Morze[2].seq[3]=1;

		// D = _..
		Morze[3].seq[0]=2;
		Morze[3].seq[1]=1;
		Morze[3].seq[2]=1;

		// E = .
		Morze[4].seq[0]=1;

		// F = .._.
		Morze[5].seq[0]=1;
		Morze[5].seq[1]=1;
		Morze[5].seq[2]=2;
		Morze[5].seq[3]=1;

		// G = _ _ .
		Morze[6].seq[0]=2;
		Morze[6].seq[1]=2;
		Morze[6].seq[2]=1;

		// H = . . . .
		Morze[7].seq[0]=1;
		Morze[7].seq[1]=1;
		Morze[7].seq[2]=1;
		Morze[7].seq[3]=1;
		
		// I = ..
		Morze[8].seq[0]=1;
		Morze[8].seq[1]=1;
		
		// J = ._ _ _
		Morze[9].seq[0]=1;
		Morze[9].seq[1]=2;
		Morze[9].seq[2]=2;
		Morze[9].seq[3]=2;
		
		// K = _._
		Morze[10].seq[0]=2;
		Morze[10].seq[1]=1;
		Morze[10].seq[2]=1;
		Morze[10].seq[3]=1;

		// L = ._..
		Morze[11].seq[0]=1;
		Morze[11].seq[1]=2;
		Morze[11].seq[2]=1;
		Morze[11].seq[3]=1;

		// M = _ _
		Morze[12].seq[0]=2;
		Morze[12].seq[1]=2;
		
		// N = _.
		Morze[13].seq[0]=2;
		Morze[13].seq[1]=1;
		
		// O = _ _ _
		Morze[14].seq[0]=2;
		Morze[14].seq[1]=2;
		Morze[14].seq[2]=2;
			
		// P = ._ _.
		Morze[15].seq[0]=1;
		Morze[15].seq[1]=2;
		Morze[15].seq[2]=2;
		Morze[15].seq[3]=1;
	
		// Q = _ _ . _
		Morze[16].seq[0]=2;
		Morze[16].seq[1]=2;
		Morze[16].seq[2]=1;
		Morze[16].seq[3]=2;

		// R = ._.
		Morze[17].seq[0]=1;
		Morze[17].seq[1]=2;
		Morze[17].seq[2]=1;
		
		// S = ...
		Morze[18].seq[0]=1;
		Morze[18].seq[1]=1;
		Morze[18].seq[2]=1;

		// T = _
		Morze[19].seq[0]=2;
		
		// U = .._
		Morze[20].seq[0]=1;
		Morze[20].seq[1]=1;
		Morze[20].seq[2]=2;

		// V = ..._
		Morze[21].seq[0]=1;
		Morze[21].seq[1]=1;
		Morze[21].seq[2]=1;
		Morze[21].seq[3]=2;

		// W = ._ _
		Morze[22].seq[0]=1;
		Morze[22].seq[1]=2;
		Morze[22].seq[2]=2;
		
		// X = _.._
		Morze[23].seq[0]=2;
		Morze[23].seq[1]=1;
		Morze[23].seq[2]=1;
		Morze[23].seq[3]=2;

		// Y = _._ _
		Morze[24].seq[0]=2;
		Morze[24].seq[1]=1;
		Morze[24].seq[2]=2;
		Morze[24].seq[3]=2;
		
		// Z = _ _ ..
		Morze[25].seq[0]=2;
		Morze[25].seq[1]=2;
		Morze[25].seq[2]=1;
		Morze[25].seq[3]=1;		
		
		// SPACE = _..._
		Morze[26].seq[0]=2;
		Morze[26].seq[1]=1;
		Morze[26].seq[2]=1;
		Morze[26].seq[3]=1;
		Morze[26].seq[4]=2;
	}
	void set_on(void)
	{
	 #ifdef RUN
		PORTB=0b11111111;
	#else
		printf("1");
	#endif
	}
	void set_off(void)
	{
	#ifdef RUN
		PORTB=0b00000000;
	#else 
		printf("0");
	#endif
	}
	void spell(char character)
	{
		int i,num;
		num=character-'A';
		//printf("%d\n",num);
		if(num<0)
			num=26;
		for( i=0;i<5;i++)
		{
			if(Morze[num].seq[i]==1)
			{
				set_on();
				
                #ifdef RUN				
				_delay_ms(DOT_LEN);
				#else
				sleep(DOT_LEN);
				#endif
				set_off();
			}
			if(Morze[num].seq[i]==2)
			{
				set_on();
				#ifdef RUN
				_delay_ms(SLASH_LEN);
				#else
				sleep(SLASH_LEN);
				#endif
				set_off();
			}
			if(Morze[num].seq[i]==0)
				continue;
			#ifdef RUN
			_delay_ms(SPACE_LEN);
			#else
			sleep(SPACE_LEN);
			#endif
			
		}
		#ifdef RUN
		_delay_ms(CHR_SPACE_LEN);
		#else
		sleep(CHR_SPACE_LEN);
		#endif
	}
	void spell_string(char *str)
	{
	//printf("%s",str);
		int i;
		for(i=0;str[i]!=0;i++)
		{
			//printf("%c",str[i]);
			spell(str[i]);
		}
	}

   int main(void)              // начало основой программы
   {
   #ifdef RUN
   DDRD = 0xff;            /* все выводы порта D сконфигурировать как выходы */
   DDRB = 0b11111111;		   /* 1 вывод порта B сконфигурировать как выходы, остальные как входы */
   DDRC = 0xff;		   /* все выводы порта C сконфигурировать как выходы */
   #endif   
convert();


	while(1)
	{
		spell_string("HELLO MY ICQ 396306841");
		#ifdef RUN
        _delay_ms(PAUSE);
        #else
		sleep(PAUSE);
		#endif
		
	}

   }      // закрывающая скобка основной программы

