/* main.c -- (c) chris kern -- Mon Feb 16 20:35:41 EST 2009     */
/* "Don't fear the pen. When in doubt, draw a pretty picture."  */
/*     -- Baker's Third Law of Design.                           */

#include <stdint.h>
#include "lcd.h"

int main(void){
  uint8_t x, y, z = 0;
  lcd_init();

  // print some text
  lcd_set_cursor(0, 11);
  lcd_putstr("128x64 LCD");
  lcd_set_cursor(2, 11);
  lcd_putstr("HELLO");
  lcd_set_cursor(3, 11);
  lcd_putstr("WORLD");

  while(1) {
    // draw an animated barberpole on the left side of the screen
    for(x = 0; x < 64; ++x) {
      for(y = 0; y < 64; ++y) {
        if ((y+x+z) % 8 < 4) {
          lcd_setbit(x, y, 1);
        } else {
          lcd_setbit(x, y, 0);
        }
      }
    }
    ++z;
    if (z == 8) z = 0;
  }
  
  return 0;
}

