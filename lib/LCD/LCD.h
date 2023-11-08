/*****************************************************************************
* | File      	:	LCD_2IN_Driver.h
******************************************************************************/
#ifndef LCD_H
#define LCD_H

#include "Config.h"

//---------------Commands list
#define displayWidth   240 //LCD width
#define displayHeight  320 //LCD height

#define NOP  0x00 //No operation
#define SWRESET  0x01 //Software reset
#define RDDID  0x04 //Read display ID
#define RDDST  0x09 // Read display status

#define SLPIN  0x10 //Sleep in
#define SLPOUT  0x11 //Sleep out
#define PTLON  0x12 //Partial mode on
#define NORON  0x13 //Partial off (Normal)

#define INVOFF  0x20 //Display inversion off
#define INVON  0x21 //Display inversion on
#define DISPOFF  0x28 //Display off
#define DISPON  0x29 //Display on

#define CASET  0x2A //Column address set
#define RASET  0x2B //Row address set
#define RAMWR  0x2C //Memory write
#define RAMRD  0x2E //Memory read

#define PTLAR  0x30 //Partial sart/end address set
#define MADCTL  0x36  //Memory data access control
#define COLMOD  0x3A //Interface pixel format

#define FRMCTR1  0xB1 //RGB Control
#define FRMCTR2  0xB2 //Porch Control
#define FRMCTR3  0xB3 //Frame Rate Control 1
#define INVCTR  0xB4
#define DISSET5  0xB6

#define GCTRL  0xB7 //Gate Control
#define GTADJ  0xB8 //Gate on timing adjustment
#define VCOMS  0xBB //VCOM Setting

#define LCMCTRL  0xC0 //LCM control
#define IDSET  0xC1 //ID setting
#define VDVVRHEN  0xC2 // VDV and VRH Command Enable
#define VRHS  0xC3 //VRH Setting
#define VDVS  0xC4 //VDV Setting
#define VMCTR1  0xC5 // VCOm Offset set
#define FRCTRL2  0xC6 //FR Control 2
#define CABCCTRL  0xC7 //CABC Control

#define RDID1  0xDA //Read ID1
#define RDID2  0xDB //Read ID2
#define RDID3  0xDC //Read ID3
#define RDID4  0xDD //Read ID4

#define GMCTRP1  0xE0 //Positive Voltage Gamma Control
#define GMCTRN1  0xE1 //Negative Voltage Gamma Control

#define PWCTR6  0xFC //NVM Setting
#define PWCTRL1 0xD0 //Power Control 1


int initDisplay(); 
void clearScreen(uint16_t Color);
void DisplayImg(uint8_t *image);
void drawPaint(uint16_t x, uint16_t y, uint16_t Color);
void HandlerLCD(int signo);

void SpiWrite(uint8_t *bString,uint8_t blen);
void setCursor(uint16_t X, uint16_t Y);
int setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void clearWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,uint16_t Color);


#endif
