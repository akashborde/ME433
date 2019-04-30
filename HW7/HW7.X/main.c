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
    char read = IMU_read();
    
    //for printing to the screen later:
    char m[100];
    int b;   
    unsigned char dat[NUM_READS]; //worked fine if unsigned
    short temp,
            x_angle, y_angle, z_angle,
            x_accel, y_accel, z_accel;
    
    while(1) 
    {
        if(read == WHOAMI_ID)
        {
            //blink LED to verify the program hasn't crashed
            if (_CP0_GET_COUNT() > 40000)
            {
                LED_Invert_A4(); //INVERT LED
               _CP0_SET_COUNT(0);
            
                                 
            IMU_read_multiple(OUT_TEMP_L, dat, NUM_READS);
            
//            temp =      (dat[1] << 8) | dat[0];
//            x_angle =   (dat[3] << 8) | dat[2];
//            y_angle =   (dat[5] << 8) | dat[4];
//            z_angle =   (dat[7] << 8) | dat[6];
            x_accel =   (dat[9] << 8) | dat[8];
            y_accel =   (dat[11] << 8) | dat[10];
            z_accel =   (dat[13] << 8) | dat[12];
            
//            sprintf(m, "temp = %d  ", temp);
//            LCD_print(m, START_X, START_Y, ILI9341_BLUE, ILI9341_PINK);
            sprintf(m, "x_accel = %d  ", x_accel);
            LCD_print(m, START_X, START_Y + 10, ILI9341_BLUE, ILI9341_PINK);
            sprintf(m, "y_accel = %d  ", y_accel);
            LCD_print(m, START_X, START_Y + 20, ILI9341_BLUE, ILI9341_PINK);
            sprintf(m, "z_accel = %d  ", z_accel);
            LCD_print(m, START_X, START_Y + 30, ILI9341_BLUE, ILI9341_PINK);
            sprintf(m, "x_accel_to_p = %d ", accel_to_percent(x_accel));
            LCD_print(m, START_X, START_Y - 10, ILI9341_BLUE, ILI9341_PINK);
            
            LCD_horizProgBar(CENTER_X, CENTER_Y + 50, 50, 5, x_accel,
                    ILI9341_ORANGE, ILI9341_BLACK);
        LCD_vertProgBar(CENTER_X - 50, CENTER_Y, 5, 50, y_accel,
                    ILI9341_ORANGE, ILI9341_BLACK);
            }
            
//            for (b = 0; b<(NUM_READS/2); b++)
//            {
//                val[b] = (dat[1+b*2] << 8 ) | dat[b*2];  
//            }
            /*
                * 0 = 1 + 0
                * 1 = 3 + 2
                * 2 = 5 + 4
             */
            
            
//            sprintf(m, "Temp = %d", val[0]); //put the string i = (number)in a char array
//            LCD_print(m, START_X, START_Y, ILI9341_BLUE, ILI9341_PINK);
//            sprintf(m, "Xaccel = %d", val[4]);
//            LCD_print(m, START_X, START_Y + 10, ILI9341_BLUE, ILI9341_PINK);
//            sprintf(m, "Yaccel = %d", val[5]);
//            LCD_print(m, START_X, START_Y + 20, ILI9341_BLUE, ILI9341_PINK);
//            sprintf(m, "Zaccel = %d", val[6]);
//            LCD_print(m, START_X, START_Y + 30, ILI9341_BLUE, ILI9341_PINK);
//            

            
//            sprintf(m, "1 = %d", 1);
//            LCD_print(m, START_X, START_Y + 10, ILI9341_BLUE, ILI9341_PINK);
//            sprintf(m, "0b011 + 1 = %d", b+1);
//            LCD_print(m, START_X, START_Y + 20, ILI9341_BLUE, ILI9341_PINK);
            
            
            
            //LCD_progBar(START_X, START_Y + 10, PROG_BAR_LEN, PROG_BAR_HEIGHT, 20, ILI9341_CYAN,ILI9341_BLACK); // draw a 30% full bar
        }
        else
        {
            sprintf(m, "Can't read from IMU");
            LCD_print(m, START_X, START_Y, ILI9341_BLUE, ILI9341_PINK);
        }  
    }
}