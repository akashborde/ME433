/* 
 * File:   main.c
 * Author: akashborde
 *
 * Created on April 16, 2019, 3:16 PM
 */


#include <stdio.h>
#include <stdlib.h>

#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro

#include "i2c_master_noint.h"
#include "pragmas.h"
#include "peripheral.h"
#include "ili9341.h"

//#include "i2c_master_noint.h" //i2c initialization File

//values for #pragma config settings are found in:
// /opt/microchip/xc32/v2.15/docs/config_docs


int main() {

    __builtin_disable_interrupts();
    initializePIC32(); //set up RAM, vector, JTAG stuff
    LED_Setup_A4();
    IMU_Setup(); // initialize the i2c
    SPI1_init();
    LCD_init();
    _CP0_SET_COUNT(0); //begin timer
    
    __builtin_enable_interrupts();    
    LCD_clearScreen(ILI9341_PINK); 
    
    //make sure the IMU is communicating
    //char read = IMU_read();
    
    //for printing to the screen later:
    char m[100];
    int b;   

    int b1x, b1y, b1w, b1h, b2x, b2y, b2w, b2h;
    
    int thresh = 1600;// button push threshhold
    int score = 0;
    
    b1x = START_X;
    b2x = START_X;
    b1y = START_Y + 100;
    b2y = START_Y + 150;
    b1w = 100;
    b2w = 100;
    b1h = 30;
    b2h = 30;
    
    unsigned short x, y;
    int z, xp, yp;
        
    sprintf(m, "INCREMENT");
    LCD_draw_button(m, b1x,b1y,b1w,b1h, ILI9341_BLUE, ILI9341_WHITE);

    sprintf(m, "DECREMENT");
    LCD_draw_button(m, b2x,b2y,b2w,b2h, ILI9341_RED, ILI9341_BLACK);
            
    
    while(1) 
    {
       
        //blink LED to verify the program hasn't crashed
        if (_CP0_GET_COUNT() > 40000)
        {
            LED_Invert_A4(); //INVERT LED
           _CP0_SET_COUNT(0);

            //IMU_read_multiple(OUT_TEMP_L, dat, NUM_READS);
            XPT2046_read(&x, &y, &z);

            LCD_posn_to_pixel(&x,&y,&xp,&yp);

            //score
            sprintf(m, "SCORE = % +.2d", score);
            LCD_print(m, ILI9341_TFTWIDTH/2 - 20, 26, ILI9341_BLUE, ILI9341_PINK); 
            
            //delete later
            sprintf(m, "x_touch %d  ", x);
            LCD_print(m, START_X, START_Y + 50, ILI9341_BLUE, ILI9341_PINK);

            sprintf(m, "xp_touch %d  ", xp);
            LCD_print(m, START_X + 100, START_Y + 50, ILI9341_BLUE, ILI9341_PINK);
           
            //delete later
            sprintf(m, "y_touch %d  ", y);
            LCD_print(m, START_X, START_Y + 60, ILI9341_BLUE, ILI9341_PINK);

            sprintf(m, "yp_touch %d  ", yp);
            LCD_print(m, START_X + 100, START_Y + 60, ILI9341_BLUE, ILI9341_PINK);

            sprintf(m, "delta_z_touch %d  ", z);
            LCD_print(m, START_X, START_Y + 70, ILI9341_BLUE, ILI9341_PINK);

            if (z > thresh)
            {
                //increment
                if((xp>b1x)&&(xp<(b1x+b1w))&&(yp>b1y)&&(yp<(b1h+b1y)))
                {
                    while (z>thresh)
                    {XPT2046_read(&x, &y, &z);}
                    score = score + 1;
                }
                if((xp>b2x)&&(xp<(b2w+b2x))&&(yp>b2y)&&(yp<(b2h+b2y)))
                {
                    while (z>thresh)
                    {XPT2046_read(&x, &y, &z);}
                    score = score - 1;
                }                
                //decrement
            }
        
        }
    }
}