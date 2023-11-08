/*****************************************************************************
* | File      	:   Config.h
******************************************************************************/
#ifndef CONFIG_H
#define CONFIG_H

#include "Debug.h"

#include "sysfsGpio.h"
#include "hardwareSPI.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
	//#define USE_DEV_LIB_PWM


#include <unistd.h>

//#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>



/**
 * data
**/


#define CS   134
#define RST  15
#define DC   118


/*------------------------------------------------------------------------------------------------------*/
uint8_t moduleInit(void);
void moduleExit(void);

int gpioMode(int pin,int mode);
void digitalWrite(int iPin, int iVal);


void SPIWrite(uint8_t* bString, uint8_t blen);

#endif
