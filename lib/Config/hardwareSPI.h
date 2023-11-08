/*****************************************************************************
* | File        :   hardwareSPI.h
******************************************************************************/
#ifndef HARDWARE_SPI
#define HARDWARE_SPI

#include <stdint.h>


int initSPI();
void SpiEnd(void);

void SpiWrite(uint8_t *bString,uint8_t blen);



#endif