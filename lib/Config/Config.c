/*****************************************************************************
* | File      	:   Config.c
******************************************************************************/
#include "Config.h"



/*****************************************
                GPIO
*****************************************/

void digitalWrite(int iPin, int iVal)
{
    GPIOWrite(iPin, iVal);
}


int GpioMode(int pin,int mode)
{
	char pOut[] = "out";
	char pIn[] = "in";
	char Cpin[9];
	sprintf(Cpin, "%d", pin);
	if (mode == 0)
	{
		initGpioC(Cpin, pIn);
	}
	else if (mode == 1)
	{
		initGpioC(Cpin, pOut);
	}
	return 0;
}


static void gpioInit(void)
{
    GpioMode(DC,1);
    //GpioMode(RST, 1);
    //GpioMode(CS, 1);
    
    
}


/******************************************************************************
function:	Module Initialize. Initialize pins and SPI protocol
parameter:
Info:
******************************************************************************/
uint8_t moduleInit(void)
{
    gpioInit();
	initSPI();
    return 0;
}

void SPIWrite(uint8_t* bString, uint8_t blen)
{
	SpiWrite(bString,blen);
}



/******************************************************************************
function:	Module exits, closes SPI and unexport pins
parameter:
Info:
******************************************************************************/
void moduleExit(void)
{
    SpiEnd();
	cleanupGPIO(DC);
	//cleanupGPIO(RST);
	//cleanupGPIO(CS);
}