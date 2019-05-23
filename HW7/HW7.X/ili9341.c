#include <xc.h>
#include "ili9341.h"
#include <math.h>

void LCD_init() 
{
    int time = 0;
    
    CS = 0; // CS
   
    LCD_command(ILI9341_SWRESET);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 7200000) {} // 300ms

    LCD_command(0xEF);
  	LCD_data(0x03);
	LCD_data(0x80);
	LCD_data(0x02);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command(0xCF);
  	LCD_data(0x00);
	LCD_data(0xC1);
	LCD_data(0x30);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command(0xED);
  	LCD_data(0x64);
	LCD_data(0x03);
	LCD_data(0x12);
    LCD_data(0x81);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command(0xE8);
  	LCD_data(0x85);
	LCD_data(0x00);
	LCD_data(0x78);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command(0xCB);
  	LCD_data(0x39);
	LCD_data(0x2C);
	LCD_data(0x00);
    LCD_data(0x34);
    LCD_data(0x02);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command(0xF7);
  	LCD_data(0x20);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command(0xEA);
  	LCD_data(0x00);
	LCD_data(0x00);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command(ILI9341_PWCTR1);
  	LCD_data(0x23);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command(ILI9341_PWCTR2);
  	LCD_data(0x10);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command(ILI9341_VMCTR1 );
  	LCD_data(0x3e);
    LCD_data(0x28);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command(ILI9341_VMCTR2);
  	LCD_data(0x86);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command(ILI9341_MADCTL);
  	LCD_data(0x48);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
  /*    
    LCD_command(ILI9341_VSCRSADD);
  	LCD_data(0x00);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
   */   
    LCD_command(ILI9341_PIXFMT);
  	LCD_data(0x55);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command(ILI9341_FRMCTR1);
  	LCD_data(0x00);
    LCD_data(0x18);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command( ILI9341_DFUNCTR);
  	LCD_data(0x08);
    LCD_data(0x82);
    LCD_data(0x27);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command(0xF2);
  	LCD_data(0); // 1
    LCD_data(0x00);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command(ILI9341_GAMMASET);
  	LCD_data(0x01);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command(ILI9341_GMCTRP1);
  	LCD_data(0x0F);
    LCD_data(0x31);
    LCD_data(0x2B);
    LCD_data(0x0C);
    LCD_data(0x0E);
    LCD_data(0x08);
    LCD_data(0x4E);
    LCD_data(0xF1);
    LCD_data(0x37);
    LCD_data(0x07);
    LCD_data(0x10);
    LCD_data(0x03);
    LCD_data(0x0E);
    LCD_data(0x09);
    LCD_data(0x00);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command(ILI9341_GMCTRN1);
  	LCD_data(0x00);
    LCD_data(0x0E);
    LCD_data(0x14);
    LCD_data(0x03);
    LCD_data(0x11);
    LCD_data(0x07);
    LCD_data(0x31);
    LCD_data(0xC1);
    LCD_data(0x48);
    LCD_data(0x08);
    LCD_data(0x0F);
    LCD_data(0x0C);
    LCD_data(0x31);
    LCD_data(0x36);
    LCD_data(0x0F);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command(0xB1);
  	LCD_data(0x00);
    LCD_data(0x10);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command(ILI9341_SLPOUT);
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    LCD_command(ILI9341_DISPON);
    
    CS = 1; // CS
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    
    CS = 0; // CS
    
    LCD_command(ILI9341_MADCTL);
    LCD_data(MADCTL_MX | MADCTL_BGR); // rotation
    time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() < time + 3600000) {} // 150ms
    
    CS = 1; // CS
}

void SPI1_init() {
  SDI1Rbits.SDI1R = 0b0100; // B8 is SDI1
  RPA1Rbits.RPA1R = 0b0011; // A1 is SDO1
  TRISBbits.TRISB7 = 0; // CS is B7
  TRISBbits.TRISB9 = 0; //CS_touch B9
  
  CS_touch = 1; //start this high
  CS = 1; // CS starts high

  // DC pin
  TRISBbits.TRISB15 = 0;
  DC = 1;
  
  SPI1CON = 0; // turn off the spi module and reset it
  SPI1BUF; // clear the rx buffer by reading from it
  SPI1BRG = 3; // this baud worked for nick, bring it down later
  SPI1STATbits.SPIROV = 0; // clear the overflow bit
  SPI1CONbits.CKE = 1; // data changes when clock goes from hi to lo (since CKP is 0)
  SPI1CONbits.MSTEN = 1; // master operation
  SPI1CONbits.ON = 1; // turn on spi1
}

unsigned char spi_io(unsigned char o) {
  SPI1BUF = o;
  while(!SPI1STATbits.SPIRBF) { // wait to receive the byte
    ;
  }
  return SPI1BUF;
}

void LCD_command(unsigned char com) {
    DC = 0; // DC
    spi_io(com);
    DC = 1; // DC
}

void LCD_data(unsigned char dat) {
    spi_io(dat);
}

void LCD_data16(unsigned short dat) {
    spi_io(dat>>8);
    spi_io(dat);
}

void LCD_setAddr(unsigned short x, unsigned short y, unsigned short w, unsigned short h) {
    LCD_command(ILI9341_CASET); // Column Address Set
    LCD_data16(x);
	LCD_data16(x+w-1); //sends the start and end posns

	LCD_command(ILI9341_PASET); // Page Address Set
	LCD_data16(y);
	LCD_data16(y+h-1); //end posn is the first + height - last (to avoid overcount)

	LCD_command(ILI9341_RAMWR); // memory Write Into RAM
}

void LCD_drawPixel(unsigned short x, unsigned short y, unsigned short color) {
  // check boundary
    
    CS = 0; // CS
    
    LCD_setAddr(x,y,1,1); //pixel is of size 1
    LCD_data16(color); //sets a color
    
    CS = 1; // CS
}

void LCD_clearScreen(unsigned short color) {
    int i;
    
    CS = 0; // CS
    
    LCD_setAddr(0,0,ILI9341_TFTWIDTH,ILI9341_TFTHEIGHT); //the whole screen

	for (i = 0;i < ILI9341_TFTWIDTH*ILI9341_TFTHEIGHT; i++){
		LCD_data16(color); // go thru and clear everything
	}
    
    CS = 1; // CS
}

void LCD_drawLetter(char letter, unsigned short x, unsigned short y, unsigned short fc, unsigned short bkc)
{
    char tbl_row = letter - 0x20; //gives you the desired row of the table
    int i,j;
    for (i = 0; i < ASCII_TBL_LENGTH; i++) //loop through each column of char elem
    {
        char col = ASCII[tbl_row][i]; //get this particular char
        for(j = 0; j < HEIGHT_CHAR; j++) //loop through each row of char elem
        {
            char bitt = (col >> j) & 1; //only want the j'th bit, clear everything else
            if (bitt == 1)
            {
                if( (x+i)<=ILI9341_TFTWIDTH && (y+j)<=ILI9341_TFTHEIGHT ) //make sure the pixel doesn't wraparound
                {
                   LCD_drawPixel(x+i,y+j, fc); //draw it if its in bounds
                }
            }
            else //bit wasn't one, so draw the background
            {
                LCD_drawPixel(x+i, y+j, bkc);
            }
            
        }
    }    
}

void LCD_print(char* m, unsigned short x, unsigned short y, unsigned short fc, unsigned short bkc)
{
    int t = 0;
    while(m[t]) //will be nonzero until the end of the string
    {
        //draw a letter with constant width (5)
        LCD_drawLetter(m[t], x+(t*ASCII_TBL_LENGTH), y, fc, bkc);
        t++;
    }    
}

void LCD_horizProgBar(unsigned short x, unsigned short y,
                 unsigned short w, unsigned short h, 
                 short a, 
                 unsigned short fc, unsigned short bkc)
{
    /*
     * Draws a bar starting at x,y with width w and height h
     * That is p percent complete     
     */
    
    int GoRight = 1;
    if (a > 0) {GoRight = -1;} //set direction as negative //this takes accel
    
    int p = accel_to_percent(a);
    
    if(p > 100) //clamp p in case its not given as a percentage
    {
        p = 100;
    }
    if(p < 0)
    {
        p = 0;
    }
    
  
    
    int i, j;
    double bar_end = (double) p*w / 100; //p percent of the total width is shaded
    
    CS = 0; //BEGIN SPI
    for(i = 0; i<w; i++) //start at x and go until x+w
    {
        for(j = 0; j < h; j++) //start at y and go until y+h
        {
            if ((double)i < bar_end)
            {
                LCD_setAddr(x+ GoRight*i,y+j,1,1); //find pixel location
                LCD_data16(fc); //sets a color
            }
            else
            {
                LCD_setAddr(x+ GoRight*i,y+j,1,1); //find pixel location
                LCD_data16(bkc); //sets background color (clears previous bar too)
            } 
            
        }
    }
    CS = 1; //end SPI
}

void LCD_vertProgBar(unsigned short x, unsigned short y,
                 unsigned short w, unsigned short h, 
                 short a, 
                 unsigned short fc, unsigned short bkc)
{
    /*
     * Draws a bar starting at x,y with width w and height h
     * That is p percent complete     
     */
    
    int GoDown = 1;
    if (a > 0) {GoDown = -1;} //set direction as negative //this takes accel
    
    int p = accel_to_percent(a);
    
    if(p > 100) //clamp p in case its not given as a percentage
    {
        p = 100;
    }
    if(p < 0)
    {
        p = 0;
    }
    
    int i, j;
    double bar_end = (double) p*h / 100; //p percent of the total width is shaded
    
    CS = 0; //BEGIN SPI
    for(i = 0; i<w; i++) //start at x and go until x+w
    {
        for(j = 0; j < h; j++) //start at y and go until y+h
        {
            if ((double)j < bar_end)
            {
                LCD_setAddr(x+i,y+GoDown*j,1,1); //find pixel location
                LCD_data16(fc); //sets a color
            }
            else
            {
                LCD_setAddr(x+i,y+GoDown*j,1,1); //find pixel location
                LCD_data16(bkc); //sets background color (clears previous bar too)
            } 
            
        }
    }
    CS = 1; //end SPI
}

int accel_to_percent(short accel)
{
    int p = abs(100* ((double) accel/ACCEL_1G)); //normalize gravity as 100% acceleration
    return p;
}

void XPT2046_read(unsigned short *x, unsigned short *y, unsigned int *z)
{
    
    
    unsigned short data;
    char read;
    
    unsigned char b1;
    unsigned char b2;
    unsigned int Z1;
    unsigned int Z2;
    
    //for the first time, we'll get an xposn
    CS_touch = 0; //select touch screen
    //first spi send is command which should return garbage
    spi_io(CMD_START | X_RQST | CMD_END);
    //second and third commands are zeroes, but they return real info
    b1 = spi_io(0x00);
    b2 = spi_io(0x00);
    CS_touch = 1; //select touch screen
    *x = (b1 << 5) | (b2 >> 3);
    
    CS_touch = 0; //select touch screen
    //first spi send is command which should return garbage
    spi_io(CMD_START | Y_RQST | CMD_END);
    //second and third commands are zeroes, but they return real info
    b1 = spi_io(0x00);
    b2 = spi_io(0x00);
    CS_touch = 1; //select touch screen
    *y = (b1 << 5) | (b2 >> 3);
    
    CS_touch = 0; //select touch screen
     //first spi send is command which should return garbage
    spi_io(CMD_START | Z1_RQST | CMD_END);
    //second and third commands are zeroes, but they return real info
    b1 = spi_io(0x00);
    b2 = spi_io(0x00);    
    CS_touch = 1; //select touch screen
    Z1 = (b1 << 5) | (b2 >> 3);
   
    
    CS_touch = 0; //select touch screen
     //first spi send is command which should return garbage
    spi_io(CMD_START | Z2_RQST | CMD_END);
    //second and third commands are zeroes, but they return real info
    b1 = spi_io(0x00);
    b2 = spi_io(0x00);    
    CS_touch = 1; //select touch screen
    Z2 = (b1 << 5) | (b2 >> 3);
    
    *z = Z1-Z2 + 4095;
    
    CS_touch = 1; //deselect the touchscreen
}

void LCD_posn_to_pixel(unsigned short *x, unsigned short *y, int *xp, int *yp)
{
    *xp = (718*(*x) - 367200)/10000;
    *yp = (-9729*(*y) + 36972000)/100000;
}

void LCD_draw_button(char *m, unsigned short x, unsigned short y, unsigned short w, 
        unsigned short h, unsigned short fc, unsigned short bkc)
{
    int i,j;
    CS = 0; //BEGIN SPI
    for(i = 0; i<w; i++) //start at x and go until x+w
    {
        for(j = 0; j < h; j++) //start at y and go until y+h
        {
                LCD_setAddr(x+i,y+j,1,1); //find pixel location
                LCD_data16(bkc); //sets a color           
        }
    }
    LCD_print(m, x+2, y+2, fc, bkc);
    CS = 1; //end SPI
    
    
}
