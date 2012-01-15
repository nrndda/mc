#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define DEVICE_ERROR 0
#define DEVICE_OK 1
#define DEVICE_TIMEOUT 2

static char * dev;
static int fd;


int zservo_opendev(char * str) {
	struct termios termios_p;
	dev = (char *)malloc(strlen(str));
	strcpy(dev, str);
	fd = open(dev, O_RDWR | O_NOCTTY);
	if(fd == -1) return DEVICE_ERROR;
	if(tcgetattr(fd, &termios_p) == -1) return DEVICE_ERROR;
	termios_p.c_cflag &= ~(CSTOPB|CS5|CS8|CS6|CS7);
	termios_p.c_cflag |= (CS8 | CREAD);
	termios_p.c_oflag |= OPOST;
	if(cfsetospeed(&termios_p, B9600) == -1) return DEVICE_ERROR;
	if(cfsetispeed(&termios_p, B9600) == -1) return DEVICE_ERROR;
	if(tcsetattr(fd, TCSANOW, &termios_p) == -1) return DEVICE_ERROR;
	return DEVICE_OK;
}

int zservo_closedev(void) {
	if(!fd) return DEVICE_ERROR;
	free(dev);
	if(close(fd) == -1) return DEVICE_ERROR;
	return DEVICE_OK;
}

int zservo_send(char * str, int len) {
	if(!fd) return DEVICE_ERROR;
	if(write(fd, str, len) < len) return DEVICE_ERROR;
	return DEVICE_OK;
}

char * zservo_devname(void) {
	return dev;
}
