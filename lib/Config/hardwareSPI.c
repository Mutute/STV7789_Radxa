/*****************************************************************************
* | File        :   hardwareSPI.c
******************************************************************************/
#include "hardwareSPI.h"


#include <stdlib.h>
#include <stdio.h>

#include <stdint.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <getopt.h> 
#include <fcntl.h> 
#include <sys/ioctl.h> 
#include <linux/types.h> 
#include <linux/spi/spidev.h> 
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <fcntl.h>
//#include <getopt.h>



static struct spi_ioc_transfer xfer;
int f_spi;

/******************************************************************************
function:   SPI port initialization
    /dev/spidev0.0 
******************************************************************************/
int initSPI()
{
	int val = 0;
	int spiMode = SPI_MODE_0; //no cs
	int spiFreq = 1000000;
	f_spi = open("/dev/spidev3.0", O_RDWR);
	if (f_spi < 0)
	{
		fprintf(stderr, "\n Failed to open SPI bus");
		return -1;
	}
	//init no cs mode
	val = ioctl(f_spi, SPI_IOC_WR_MODE, &spiMode);
	if (val == -1)
	{
		fprintf(stderr, "\n Failed to setup SPI mode");
	}
	memset(&xfer, 0, sizeof(xfer));
	xfer.speed_hz = spiFreq;
	xfer.cs_change = 0;
	xfer.delay_usecs = 0;
	xfer.bits_per_word = 8;
	return 0;
}



/******************************************************************************
function:   SPI device End
parameter:
Info:
******************************************************************************/
void SpiEnd(void)
{
    if (close(f_spi) != 0){
        printf("Failed to close SPI device.\n");  
    }
}



/******************************************************************************
function: SPI port sends blen byte(s) of data
parameter:
    buf :   Sent data
Info:
******************************************************************************/
void SpiWrite(uint8_t *bString,uint8_t blen)
{
	int relVal=0;
	//set pin cs to 0 (if cs mode enable)
	xfer.rx_buf=0;
	xfer.tx_buf=(unsigned long)bString;
	xfer.len=blen;
	relVal=ioctl(f_spi,SPI_IOC_MESSAGE(1),&xfer);
	if (relVal == 0) {
		printf("Failed to send data.\n");
	}
	//set pin add cs to 1 (if cs mode enable)
	
}

