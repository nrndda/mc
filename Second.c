/*********************************************
* vim: set sw=8 ts=8 si :
* Author: Guido Socher, Copyright: GPL 
* This program is to test the led connected to
* PD5. 
* Chip type           : AT90S4433
* Clock frequency     : 4,000000 MHz
*********************************************/
#include <avr/io.h>

void delay_ms(unsigned short ms)
/* delay for a minimum of <ms> */
/* with a 4Mhz crystal, the resolution is 1 ms */
{
unsigned short outer1, outer2;
outer1 = 200; 
while (outer1) {
			outer2 = 1000;
				while (outer2) {
					    while ( ms ) ms--;
							outer2--;
								}
									outer1--;
									    }
									    }
									    
									    void main(void)
									    {
										/* enable  PD5 as output */
										    sbi(DDRD,PD5);
											while (1) {
												/* led on, pin=0 */
													cbi(PORTD,PD5);
														delay_ms(500);
															/* set output to 5V, LED off */
																sbi(PORTD,PD5);
																	delay_ms(500);
																	    }
																	    }
																	    