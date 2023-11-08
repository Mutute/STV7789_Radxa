/*****************************************************************************
* | File        :   sysfs_gpio.h
******************************************************************************/
#ifndef SYSFS_GPIO_
#define SYSFS_GPIO_

#include <stdio.h>

#define SYSFS_GPIO_IN  0
#define SYSFS_GPIO_OUT 1



int cleanupGPIO(int iPin);
int initGpioC(char* pin,char* mode);
int GPIOWrite(int Pin, int value);

#endif