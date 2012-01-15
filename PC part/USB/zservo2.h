#if !defined(__ZDEFS_H)
#include "zdefs.h"
#endif

class ZServo {
private:
	char * dev;
	int fd;
public:
	int opendev(char * str);
	int closedev(void);
	int send(char * str, int len);
	char * devname(void);
};

int ZServo::opendev(char * str) {
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

int ZServo::closedev(void) {
	if(!fd) return DEVICE_ERROR;
	free(dev);
	if(close(fd) == -1) return DEVICE_ERROR;
	return DEVICE_OK;
}

int ZServo::send(char * str, int len) {
	if(!fd) return DEVICE_ERROR;
	if(write(fd, str, len) < len) return DEVICE_ERROR;
	return DEVICE_OK;
}

char * ZServo::devname(void) {
	return dev;
}

