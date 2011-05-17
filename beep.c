   #include <avr/io.h>
   #include <util/delay.h>
 
void beep(float ms, float freq)
{
  float t=0,dt=0;
  dt=1000/(float)freq;
  while(t<ms)
  {
    t=t+dt;
    PORTC=0b00000001;
    _delay_ms(dt/2);
    PORTC=0b00000000;
    _delay_ms(dt/2);
  }
    
}
 
int main(void)
{
  DDRC = 0xff;
  beep(400,2000.0);
  beep(200,2300.0);
  beep(200,2000.0);
  beep(200,2500.0);
  beep(200,2350.0);

  _delay_ms(500);
  
  beep(400,2000.0);
  beep(200,2300.0);
  beep(200,2000.0);
  beep(200,2500.0);
  beep(200,2350.0);
  
}
