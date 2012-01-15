#include <zservo.h>
typedef unsigned int uint;

int main(void) {
	char buf[6];
	uint i, ii;
	zservo_opendev("/dev/cuad1");
	for(i = 0; i < 20; i++) {
		for(ii=0;ii<sizeof(buf);ii++) 
			buf[ii] = rand() % 100 - 50, printf("%d ", buf[ii]);
		printf("\n");
		zservo_send(buf, sizeof(buf));
		sleep(1);
	}
	zservo_closedev();
	return 0;
}

