#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#define LED_MAGIC 0x66
#define LED_ON  _IOW(LED_MAGIC, 0, int)
#define LED_OFF _IOW(LED_MAGIC, 1, int)

int main(int argc, char **argv)
{
	int fd;

	fd = open("/dev/led", O_RDWR);
	if(fd<0){
		perror("open led failed\n");
		exit(1);
	}

	while(1){
		ioctl(fd,LED_ON);
		usleep(100000);
		ioctl(fd,LED_OFF);
		usleep(100000);
	}
	
	return 0;
}
