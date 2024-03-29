#define RUN
#ifdef RUN
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> 

#else
#include <stdio.h>
#include <dos.h>
char PORTB, DDRB,DDRC,DDRD;
#endif

// ������������ ���-�� �������� ���������
#define NMAX 64

int N_pixels=0; // ���������� ��������� ��������
typedef struct 
{
    int x, y, color;
    char on;
} pixel;

pixel P[NMAX];

// ���������� ������
// �������� ������� �� ������������ ���������� �������
// � ����������� �� ���-�� �������� ���������
void Refresh(
            int time_ms // ����� ������� �� ���� ����������
            )
{
    int time=time_ms/N_pixels; //������� ����� ���� ������
    int i;
    for(i=0;i<NMAX;i++)
    {
        if(P[i].on==1)
        {
                PORTA=1<<P[i].x;
                PORTC=0xff ^ (1<<P[i].y);
                #ifdef RUN
                _delay_ms(time);
                #else
                sleep(time);
                #endif
        }
    }
    
}

// �������� ������ �� �����
void AddPixel(int x, int y)
{
    int i;
    // ���� ������ ����������� ������
    for(i=0;i<NMAX;i++)
    {
        if(P[i].on==0)
        {
                break;
        }
    }
    P[i].x=x;
    P[i].y=y;
    P[i].on=1;
    //P[i].color=color;
    N_pixels++;
}

// ��������� ������ � ������� ������������
void RemovePixel( int x, int y)
{
    int i;
    // ���� ������ � ������� ������������
    for(i=0;i<NMAX;i++)
    {
        if(P[i].x==x && P[i].y==y && P[i].on==1)
        {
                break;
        }
    }
    P[i].on=0;
    N_pixels--;
}

// ������ �������
void InitSmile(void)
{
    // �����
    AddPixel(3,2);

    AddPixel(6,2);
    
    
    // ���
    AddPixel(2,5);
    AddPixel(3,6);
    AddPixel(4,7);
    AddPixel(5,7);
    AddPixel(6,6);
    AddPixel(7,5);
}

int main(void)              // ������ ������� ���������
   {
   #ifdef RUN
   DDRB = 0xff;            /* ��� ������ ����� D ���������������� ��� ������ */
   DDRA = 0xff;		   /* 1 ����� ����� B ���������������� ��� ������, ��������� ��� ����� */
   DDRC = 0xff;		   /* ��� ������ ����� C ���������������� ��� ������ */
   PORTA = 0b00010000;
   PORTC = 0b11101111;
   PORTB = 0x00;
   #endif   
   
   InitSmile();
   while(1)
   {
		
	   Refresh(0);
				
   } 

}      // ����������� ������ �������� ���������
