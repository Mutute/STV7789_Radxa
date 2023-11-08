/*****************************************************************************
* | File      	:	LCD.c
******************************************************************************/
#include "LCD.h"
#include <string.h>
#include <stdlib.h>		//itoa()
/*******************************************************************************
function:
	Hardware reset
*******************************************************************************/

/*
static void LCDReset(void)
{
	digitalWrite(CS, 1);
	sleep(100);
	digitalWrite(RST, 0);
	sleep(100);
	digitalWrite(RST, 1);
	sleep(100);
}
*/


/*******************************************************************************
function:
		Write data and commands
*******************************************************************************/



int sendData(uint8_t data)
{
	//char high[]="1";
	uint8_t sData[1];
	sData[0]=data;
	digitalWrite(DC,1); //DC High for Data
	SPIWrite(sData,1);
	return 1;
}
int sendCommand(uint8_t command)
{
	//char low[]="0";
	uint8_t sCommande[1];
	sCommande[0]=command;
	digitalWrite(DC,0); //DC Low for Command
	SPIWrite(sCommande,1);
	return 1;
	
}  

void sendData16bits(uint16_t data)
{
	
	digitalWrite(DC, 1);
	uint8_t image[2];
	image[0] = data >> 8;
	image[1] = data;
	uint8_t* p = (uint8_t*)(image);
	SPIWrite(p, 2);
}	 

/******************************************************************************
function:	
		Common register initialization
******************************************************************************/
int initDisplay()
{

	//reset();
	sendCommand(MADCTL); //Memory data access control
	sendData(0x00);  

	sendCommand(COLMOD); //Interface pixel format
	sendData(0x05);

	sendCommand(INVON); //Display inversion on

	sendCommand(CASET); //Column address set
	sendData(0x00);
	sendData(0x00);
	sendData(0x01); 
	sendData(0x3F);

	sendCommand(RASET); //Row address set
	sendData(0x00);
	sendData(0x00);
	sendData(0x00);
	sendData(0xEF);

	sendCommand(FRMCTR2); //Frame rate ctrl - idle mode
	sendData(0x0C);
	sendData(0x0C);
	sendData(0x00);
	sendData(0x33);
	sendData(0x33);

	sendCommand(GCTRL); //Gate Control
	sendData(0x35); 

	sendCommand(VCOMS); //VCOM Setting
	sendData(0x1F);

	sendCommand(LCMCTRL); //Power control
	sendData(0x2C);

	sendCommand(RAMWR); //Memory write
	sendData(0x01);

	sendCommand(VRHS); //VRH Setting
	sendData(0x12);   

	sendCommand(VDVS); //VDV Setting
	sendData(0x20);

	sendCommand(FRCTRL2); //FR Control 2
	sendData(0x0F); 

	sendCommand(PWCTRL1); //Power Control 1
	sendData(0xA4);
	sendData(0xA1);

	sendCommand(GMCTRP1); //Positive Voltage Gamma Control
	sendData(0xD0);
	sendData(0x08);
	sendData(0x11);
	sendData(0x08);
	sendData(0x0C);
	sendData(0x15);
	sendData(0x39);
	sendData(0x33);
	sendData(0x50);
	sendData(0x36);
	sendData(0x13);
	sendData(0x14);
	sendData(0x29);
	sendData(0x2D);

	sendCommand(GMCTRN1); //Negative Voltage Gamma Control
	sendData(0xD0);
	sendData(0x08);
	sendData(0x10);
	sendData(0x08);
	sendData(0x06);
	sendData(0x06);
	sendData(0x39);
	sendData(0x44);
	sendData(0x51);
	sendData(0x0B);
	sendData(0x16);
	sendData(0x14);
	sendData(0x2F);
	sendData(0x31);
	sendCommand(INVON); //Invert display

	sendCommand(SLPOUT); ////Sleep out

	sendCommand(DISPON); //Display on


return 1;
}

/******************************************************************************
function:	Set the cursor position
parameter	:
	  x0	: 	Start  x coordinate
	  Y0	:	Start  y coordinate
	  X1	:	End x coordinates
	  Y1	:	End y coordinatesen
******************************************************************************/
int setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{ 

        sendCommand(CASET);      // Column addr set
        sendData(x0 >> 8);
        sendData(x0 & 0xFF);             // XSTART
        sendData((x1 - 1) >> 8);
        sendData((x1 - 1) & 0xFF);             // XEND
        sendCommand(RASET);       // Row addr set
        sendData(y0 >> 8);
        sendData(y0 & 0xFF);             // YSTART
        sendData((y1 - 1) >> 8);
        sendData((y1 - 1)  & 0xFF);             // YEND
        sendCommand(RAMWR);       // write to RAM
		return 1;
}

/******************************************************************************
function:	Settings window
parameter	:
	  X		: 	Start x coordinate
	  Y		:	Start y coordinate

******************************************************************************/
void setCursor(uint16_t X, uint16_t Y)
{ 

	sendCommand(CASET); // Column addr set
	sendData(X >> 8);
	sendData(X);
	sendData(X >> 8);
	sendData(X);

	sendCommand(RASET);  // Row addr set
	sendData(Y >> 8);
	sendData(Y);
	sendData(Y >> 8);
	sendData(Y);

	sendCommand(RAMWR); // write to RAM
}

/******************************************************************************
function:	Clear screen function, refresh the screen to a certain color
parameter	:
	  Color :		The color you want to clear all the screen
******************************************************************************/
void clearScreen(uint16_t Color)
{
	uint8_t i;
	uint8_t image[displayWidth];
	for(i=0;i<displayWidth; i += 2){
		image[i] = Color >> 8;
		image[i + 1] = Color;
	}
	uint8_t *p= (uint8_t *)(image);
	setWindow(0,0,displayWidth,displayHeight);
	digitalWrite(DC, 1);

	for(int j = 0; j < displayHeight*2; j++){
		SPIWrite(p,displayWidth);
	}
}

/******************************************************************************
function:	Refresh a certain area to the same color
parameter	:
	  x0	: 	Start  x coordinate
	  Y0	:	Start  y coordinate
	  X1	:	End x coordinates
	  Y1	:	End y coordinatesen
	  color :	Set the color
******************************************************************************/

void clearWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,uint16_t Color)
{          
	uint16_t i,j; 
	uint8_t image[2];
	image[0] = Color >> 8;
	image[1]= Color;
	setWindow(x0,y0,x1-1,y1-1);
    digitalWrite(DC, 1);
	uint8_t *p= (uint8_t *)(image);
	for(i = y0; i <= y1-1; i++){
		for(j = x0; j <= x1-1; j++){
			SPIWrite(p,2);
		}
	}
}

/******************************************************************************
function: Show a picture
parameter	:
		image: Picture buffer
******************************************************************************/
void DisplayImg(uint8_t *image)
{
	uint16_t i;
	setWindow(0, 0, displayWidth, displayHeight);
	digitalWrite(DC, 1);
	for(i = 0; i < displayHeight*2; i++){
		SPIWrite((uint8_t *)image+displayWidth*i,displayWidth);
	}
}

/******************************************************************************
function: Draw a point
parameter	:
	    x	: 	Set the x coordinate
	    y	:	Set the y coordinate
	  Color :	Set the color
******************************************************************************/
void drawPaint(uint16_t x, uint16_t y, uint16_t Color)
{
	setCursor(x, y);
	sendData16bits(Color); 	    
}

void  HandlerLCD(int signo)
{
    //System Exit
    printf("\r\nHandler:Program stop\r\n");     
    moduleExit();
	exit(0);
}
