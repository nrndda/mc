#include "zdrone/zservo.h"
typedef unsigned int uint;

int main(void) {
	ZServo servo;
	char buf[6];
	servo.opendev("/dev/cuad1");
	for(uint i = 0; i < 20; i++) {
		for(uint ii=0;ii<sizeof(buf);ii++) 
			buf[ii] = rand() % 100 - 50, printf("%d ", buf[ii]);
		printf("\n");
		servo.send(buf, sizeof(buf));
		sleep(1);
	}
	servo.closedev();
	return 0;
}
