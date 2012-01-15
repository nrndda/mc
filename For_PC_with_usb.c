   #include <avr/io.h>
   #include <util/delay.h>
   #include <avr/interrupt.h>
   #include <stdlib.h>
   #include <stdio.h>
   #include <string.h>
	char * string, sreg;
	char color[ ] = {'r','g','o'};
	int fpower(int number,int power);
	void EEPROM_write(unsigned int uiAddress, unsigned char ucData);
	unsigned char EEPROM_read(unsigned int uiAddress);
	unsigned char USART_Receive( void );
	void USART_Transmit( unsigned char data );

	void Put_Pixel(unsigned char Pixel_Position_X, unsigned char Pixel_Position_Y,char Pixel_color, float delay)
	{
		PORTA = 0x00;
		PORTC = 0xff;
		PORTD = PORTD | 0xf0;
		PORTB = PORTB | 0x0f;
		if ((Pixel_Position_X < 8) && (Pixel_Position_Y < 8)) {
		    PORTA = fpower(2,Pixel_Position_X);
		    if ((Pixel_color == 'g') || (Pixel_color == 'o')) PORTC = ~fpower(2,Pixel_Position_Y);
		    if ((Pixel_color == 'r') || (Pixel_color == 'o')) {
		      if ((7 - Pixel_Position_Y) <= 3) PORTB = PORTB & ~fpower(2,7-Pixel_Position_Y);
		      else  PORTD = PORTD & ~fpower(2,7 - Pixel_Position_Y);
		    }
		    _delay_ms(delay);
		}
	}

/*	SIGNAL (SIG_OVERFLOW0)
	{

	}

	SIGNAL (SIG_INTERRUPT0)
	{

	}
*/
	int main(void)
	{
		DDRC = 0xff;
		PORTC = 0x00;

		DDRD = 0xff;
		PORTD = 0x00;

		DDRB = 0xff;
		PORTB = 0x00;

		DDRA = 0xff;
		PORTA = 0x00;
		//SFIOR = (SFIOR & !0x04);
		//ACSR = 0x80; // Analog Comparator off
		//ADCSRA = 0x00; // Analog-to-Digital Converter off
		//GIMSK = 0x00;
		//TCCR0 = 0b00000001;	//Timer0 clk/1
		//TCNT0 = 0x00;		//Timer0 start point
		//TIMSK = (1 << TOIE0);	//Timer0 on
		//MCUCR = 0b00000001;	/* Прерывание от INT0 генерируется изменением напряжения, а на INT1 низким напряжением */
		//GICR  = 0b01000000;
		UBRRH = 0;	// Делитель частоты
		UBRRL = 8;	// для USART
		//UCSRA = (1<<U2X);
		UCSRB = (1<<RXEN);//|(1<<TXEN);
		UCSRC = (1<<URSEL)|(0<<USBS)|(3<<UCSZ0);

		//sei();
		Put_Pixel(0,0,'g',3);
		Put_Pixel(0,7,'g',3);
		Put_Pixel(7,0,'g',3);
		Put_Pixel(7,7,'g',3);
		Put_Pixel(8,8,'g',3);
		Put_Pixel(0,0,'r',3);
		Put_Pixel(0,7,'r',3);
		Put_Pixel(7,0,'r',3);
		Put_Pixel(7,7,'r',3);
		Put_Pixel(8,8,'g',3);
		Put_Pixel(0,0,'o',3);
		Put_Pixel(0,7,'o',3);
		Put_Pixel(7,0,'o',3);
		Put_Pixel(7,7,'o',3);
//		unsigned char data;
		//string = (char *)malloc((D_SZ+1)*sizeof(char));
		int m = 0;
 		for (;;)
		{
//		  data = USART_Receive();
//		  _delay_ms(10);
//		  Put_Pixel(data, 'r');
//		  _delay_ms(50);
//		  USART_Transmit(data);
//		  _delay_ms(10);
		  //Put_Pixel(3,'r');
		  //_delay_ms(200);
		  //Put_Pixel(3,'g');
		  //_delay_ms(200);
		  //Put_Pixel(3,'o');
		  //_delay_ms(200);
		  for (float b=0.2;b<=0.8;b+=0.3)
		  {
		    for (int k=0;k<3;k++)
		      for (int i=0;i<8;i++)
			  {
			    Put_Pixel(i,i,color[k],b);
			    Put_Pixel(7-i,7-i,color[k],b);
			    Put_Pixel(7-i,i,color[k],b);
			    Put_Pixel(i,7-i,color[k],b);
			  }
		    for (int k=1;k>-1;k--)
		      for (int i=7;i>-1;i--)
			  {
			    Put_Pixel(i,i,color[k],b);
			    Put_Pixel(7-i,7-i,color[k],b);
			    Put_Pixel(7-i,i,color[k],b);
			    Put_Pixel(i,7-i,color[k],b);
			  }
		  }
		  for (float b=0.0001;b<=0.0003;b+=0.0001)
		  for (int i=7;i>-1;i--)
		  {
		    if (i>=4) m = i;
		    else m = 7-i;
		    for (int k=0;k<10+5*b+5*m;k++)
		    for (int j=7-m;j<=m;j++)
		    {
		      Put_Pixel(i,j,color[i%3],b);
		      Put_Pixel(7-i,7-j,color[i%3],b);
		    }
		  }
		  for (float b=0.0001;b<=0.0003;b+=0.0001)
		    for (int i=7;i>-1;i--)
		    {
		      if (i>=4) m = i;
		      else m = 7-i;
		      for (int k=0;k<10+5*b+5*m;k++)
			for (int j=7-m;j<=m;j++)
			{
			  Put_Pixel(j,i,color[i%3],b);
			  Put_Pixel(7-j,7-i,color[i%3],b);
			}
		    }


		    for (float b=0.005;b<=0.015;b+=0.005)
		      for (int i=7;i>-1;i--)
		      {
			if (i>=4) m = i;
			else m = 7-i;
			for (int k=0;k<5+10*b+2*m;k++)
			  for (int j=7-m;j<=m;j++)
			  {
			    Put_Pixel(j,i,color[i%3],b*(4/m));
			    Put_Pixel(i,j,color[i%3],b*(4/m));
			    Put_Pixel(7-j,7-i,color[i%3],b*(4/m));
			    Put_Pixel(7-i,7-j,color[i%3],b*(4/m));
			  }
		      }

		    for (float b=0.5;b<=1.2;b+=0.3)
		    {
		      for (int i=0;i<4;i++)
		      {
			for (int j=i;j<=6-i;j++)
			{
			  Put_Pixel(i,j,color[j%3],b);
			}
			for (int j=i;j<=6-i;j++)
			{
			  Put_Pixel(j,7-i,color[j%3],b);
			}
			for (int j=i;j<=6-i;j++)
			{
			  Put_Pixel(7-i,7-j,color[j%3],b);
			}
			for (int j=i;j<=6-i;j++)
			{
			  Put_Pixel(7-j,i,color[j%3],b);
			}
		      }

			for (int i=3;i>-1;i--)
			{
			  for (int j=6-i;j>=i;j--)
			  {
			    Put_Pixel(7-j,i,color[j%3],b);
			  }
			  for (int j=6-i;j>=i;j--)
			  {
			    Put_Pixel(7-i,7-j,color[j%3],b);
			  }
			  for (int j=6-i;j>=i;j--)
			  {
			    Put_Pixel(j,7-i,color[j%3],b);
			  }
			  for (int j=6-i;j>=i;j--)
			  {
			    Put_Pixel(i,j,color[j%3],b);
			  }
			}
		      }

		  for (float b=0.001;b<=0.003;b+=0.001)
		    for (int i=0;i<=15;i++)
		    {
		      int t;
		      if (i <= 7) t = i;
		      else t = 7;
			for (int n=0;n<=10+10*b+5*(8-t);n++)
			  for (int j=t;j>=0;j--)
			    for (int k=0;k<=t;k++)
			      if ((k + j) == i)  {
				Put_Pixel(k,j,color[i%3],b);
				Put_Pixel(7-k,7-j,color[i%3],b);
			      }
		    }

		  for (float b=0.001;b<=0.003;b+=0.001)
		    for (int i=0;i<=15;i++)
		    {
		      int t;
		      if (i <= 7) t = i;
		      else t = 7;
		      for (int n=0;n<=10+10*b+5*(8-t);n++)
			for (int j=t;j>=0;j--)
			  for (int k=0;k<=t;k++)
			    if ((k + j) == i)  {
			      Put_Pixel(7-k,j,color[i%3],b);
			      Put_Pixel(k,7-j,color[i%3],b);
			    }
		    }

		  for (float b=0.01;b<=0.03;b+=0.01)
		    for (int j=0;j<=20;j++)
		    {
		      for (int i=0;i<=7;i++)
			for (int k=0;k<=7;k++)
			    Put_Pixel(i,k,color[(i+k+j)%3],b);
			Put_Pixel(8,8,color[1],b);
			_delay_ms(5);
		    }
		//	strcpy(string," Smax,R:");

		}
		//free(string);
		return 0;
   	}

unsigned char USART_Receive( void )
	{
	  /* Wait for data to be received */
	  while ( !(UCSRA & (1<<RXC)) )
	    ;
	  /* Get and return received data from buffer */
	  return UDR;
	}

void USART_Transmit( unsigned char data )
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

void EEPROM_write(unsigned int uiAddress, unsigned char ucData)
	{
	  /* Wait for completion of previous write */
	  while(EECR & (1<<EEWE));
	  /* Set up address and data registers */
	  EEAR = uiAddress;
	  EEDR = ucData;
	  sreg=SREG;
	  cli();
	  /* Write logical one to EEMWE */
	  EECR |= (1<<EEMWE);
	  /* Start eeprom write by setting EEWE */
	  EECR |= (1<<EEWE);
	  SREG=sreg;
	}

unsigned char EEPROM_read(unsigned int uiAddress)
	{
	  /* Wait for completion of previous write */
	  while(EECR & (1<<EEWE));
	  /* Set up address register */
	  EEAR = uiAddress;
	  sreg=SREG;
	  cli();
	  /* Start eeprom read by writing EERE */
	  EECR |= (1<<EERE);
	  /* Return data from data register */
	  SREG=sreg;
	  return EEDR;
	}

