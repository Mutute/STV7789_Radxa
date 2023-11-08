/*****************************************************************************
* | File        :   SYSFS_GPIO.c
******************************************************************************/
#include "sysfsGpio.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int cleanupGPIO(int iPin)
{
    // Unexport the desired pin by writing to /sys/class/gpio/unexport
    //char path[42];
	char pin[9];
	sprintf(pin, "%d", iPin);
    size_t leng = strlen(pin);
    int fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (fd == -1) {
        printf("Unable to open /sys/class/gpio/unexport");
        exit(1);
    }

    if (write(fd, pin, leng) != strlen(pin)) {
        printf("Error writing to /sys/class/gpio/unexport");
        exit(1);
    }

    close(fd);

    return 0;
}

int initGpioC(char* pin,char* mode)
{
	   // Export the desired pin by writing to /sys/class/gpio/export
	char path[42];
    size_t leng=strlen(pin);
    int fd = open("/sys/class/gpio/export", O_WRONLY);
    if (fd == -1) {
        printf("Unable to open /sys/class/gpio/export");
        exit(1);
    }

    if (write(fd, pin, leng) != strlen(pin)) {
        printf("Error writing to /sys/class/gpio/export");
        exit(1);
    }

    close(fd);

    // Set the pin to be an output by writing "out" to /sys/class/gpio/gpio24/direction
	sprintf(path, "/sys/class/gpio/gpio%s/direction", pin);
    fd = open(path, O_WRONLY);
    if (fd == -1) {
        printf("Unable to open /sys/class/gpio/gpio%s/direction",pin);
        exit(1);
    }

    if (write(fd, mode, strlen(mode)) != strlen(mode)) {
        printf("Error writing to /sys/class/gpio/gpio%s/direction",pin);
        exit(1);
    }

    close(fd);

    
	return 0;
}


int GPIOWrite(int Pin, int value)
{
	char path[42];
	char pin[9];
	sprintf(pin, "%d", Pin);
	char val[3];
	sprintf(val, "%d", value);
	
	sprintf(path, "/sys/class/gpio/gpio%s/value", pin);
    int fd = open(path, O_WRONLY);
    if (fd == -1) {
        printf("Unable to open /sys/class/gpio/gpio%s/value",pin);
        exit(1);
    }


    if (write(fd, val, 1) != 1) {
            printf("Error writing to /sys/class/gpio/gpio%s/value",pin);
            exit(1);
        }

    close(fd);
    return 0;
}
