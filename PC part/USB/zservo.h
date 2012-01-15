#ifndef __ZSERVO_H
#define __ZSERVO_H

#define DEVICE_ERROR 0
#define DEVICE_OK 1
#define DEVICE_TIMEOUT 2

extern int zservo_opendev(char * str);
extern int zservo_closedev(void);
extern int zservo_send(char * str, int len);
extern char * zservo_devname(void);

#endif
