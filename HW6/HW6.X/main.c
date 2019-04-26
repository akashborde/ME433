/* 
 * File:   main.c
 * Author: akashborde
 *
 * Created on April 24, 2019, 4:32 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "pragmas.h" //pic32 setup
#include "peripheral.h" //setup for peripherals
#include <xc.h> //SFR definitions
#include <sys/attribs.h> // __ISR macro (not sure)
#include "ili9341.h" //LCD 

#define COUNTS_PER_SEC 24000000
//one count every 0.0000000416 sec
//24000000 counts / sec


int main() 
{
    __builtin_disable_interrupts();
    initializePIC32(); //sets up RAM, vector, JTAG stuff   
    LED_Setup_A4();   //check if the pic hangs in code
    SPI1_init();
    LCD_init();
    __builtin_enable_interrupts();
    
    LCD_clearScreen(ILI9341_PINK); 
     
    _CP0_SET_COUNT(0);
    int i = 0;
    int c1, c2;
    double frames_sec;
    char m[100];
       
    while(1)
    {    
        
        if((_CP0_GET_COUNT() > TEN_HZ_PERIOD) && (i < 101)) //check if the pic hangs
        {    
            c1 = _CP0_GET_COUNT();
            sprintf(m, "Hello, world %d", i); //put the string i = (number) in a char array
            LCD_print(m, START_X, START_Y, ILI9341_BLUE, ILI9341_PINK);
            LCD_progBar(START_X, START_Y + 10, PROG_BAR_LEN, PROG_BAR_HEIGHT, i, ILI9341_CYAN,ILI9341_BLACK); // draw a 30% full bar            
            c2 = _CP0_GET_COUNT();
            
            frames_sec = (double) COUNTS_PER_SEC / (c2-c1);
            sprintf(m, "Frame Rate = %.2f frames/sec", frames_sec); 

            LCD_print(m, START_X, START_Y + 20, ILI9341_BLUE, ILI9341_PINK);
            LED_Invert_A4();
           _CP0_SET_COUNT(0);
           i++;
        }
    }
    
}