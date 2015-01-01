#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#ifndef LED_STRIP_CTRL_PORT
#define LED_STRIP_CTRL_PORT 3
#endif

// LED strip Data port: PORTB[3]
#ifndef LED_STRIP_DATA_PORT
#define LED_STRIP_DATA_PORT PORTB
#endif

#ifndef LED_STRIP_DATA_DDR
#define LED_STRIP_DATA_DDR DDRB
#endif

#define LED_STRIP_SINGLE_COLOR_DATA_WIDTH 8
#define LED_STRIP_SINGLE_LEDS_NUM 3
#define LED_STRIP_SINGLE_DATA_WIDTH LED_STRIP_SINGLE_LEDS_NUM*LED_STRIP_SINGLE_COLOR_DATA_WIDTH
#define LED_STRIP_TIME_MARGIN 1.0

//High Speed operation
#define HS_delay_ZERO_high 0.25 //0.25us
#define HS_delay_ZERO_low 1.0 //1.0us
#define HS_delay_ONE_high 0.6 //0.6us
#define HS_delay_ONE_low 0.65 //0.65us
#define HS_delay_RESET 50 //50us

//Low Speed operation
#define LS_delay_ZERO_high 2*HS_delay_ZERO_high //in datasheet 0.5us
#define LS_delay_ZERO_low 2*HS_delay_ZERO_low //in datasheet 2.0us
#define LS_delay_ONE_high 2*HS_delay_ONE_high //in datasheet 1.2us
#define LS_delay_ONE_low 2*HS_delay_ONE_low //in datasheet 1.3us
#define LS_delay_RESET HS_delay_RESET //50us

#define HIGH_SPEED_MODE

#ifdef HIGH_SPEED_MODE
#define delay_ZERO_high HS_delay_ZERO_high
#define delay_ZERO_low HS_delay_ZERO_low
#define delay_ONE_high HS_delay_ONE_high
#define delay_ONE_low HS_delay_ONE_low
#define delay_RESET HS_delay_RESET
#else
#define delay_ZERO_high LS_delay_ZERO_high
#define delay_ZERO_low LS_delay_ZERO_low
#define delay_ONE_high LS_delay_ONE_high
#define delay_ONE_low LS_delay_ONE_low
#define delay_RESET LS_delay_RESET
#endif

#define BIT_INVERSE(value,bit) ((value) ^= (1 << (bit)))
#define BIT_SET(value,bit) ((value) |= (1 << (bit)))
#define BIT_CLEAR(value,bit) ((value) &= (~(1 << (bit))))
#define IS_BIT_SET(value,bit) ((((value) & (1 << (bit))) != 0) ? 1 : 0)

void led_strip_init(void)
{
  BIT_SET(LED_STRIP_DATA_DDR,LED_STRIP_CTRL_PORT);
  BIT_CLEAR(LED_STRIP_DATA_PORT,LED_STRIP_CTRL_PORT);
}

void send_one(void)
{
  BIT_SET(LED_STRIP_DATA_PORT,LED_STRIP_CTRL_PORT);
  _delay_us(delay_ONE_high * LED_STRIP_TIME_MARGIN);
  BIT_CLEAR(LED_STRIP_DATA_PORT,LED_STRIP_CTRL_PORT);
  _delay_us(delay_ONE_low * LED_STRIP_TIME_MARGIN);
}

void send_zero(void)
{
  BIT_SET(LED_STRIP_DATA_PORT,LED_STRIP_CTRL_PORT);
  _delay_us(delay_ZERO_high * LED_STRIP_TIME_MARGIN);
  BIT_CLEAR(LED_STRIP_DATA_PORT,LED_STRIP_CTRL_PORT);
  _delay_us(delay_ZERO_low * LED_STRIP_TIME_MARGIN);
}

void led_strip_reset(void)
{
  BIT_CLEAR(LED_STRIP_DATA_PORT,LED_STRIP_CTRL_PORT);
  _delay_us(delay_RESET * LED_STRIP_TIME_MARGIN);
}

void set_led(unsigned char red, unsigned char green, unsigned char blue)
{
  unsigned char color_cur;
  for (unsigned char color=0;color<LED_STRIP_SINGLE_LEDS_NUM;color++)
  {
    switch (color)
    {
      case 0: color_cur = green; break;
      case 1: color_cur = red; break;
      case 2: color_cur = blue; break;
      default: color_cur = green; break;
    }
    for (unsigned char color_bit=LED_STRIP_SINGLE_COLOR_DATA_WIDTH;color_bit>0;color_bit--)
      if IS_BIT_SET(color_cur,color_bit-1)
        send_one();
      else
        send_zero();
  }
}
