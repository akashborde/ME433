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
#include <math.h>

//#include "i2c_master_noint.h" //i2c initialization File

//values for #pragma config settings are found in:
// /opt/microchip/xc32/v2.15/docs/config_docs

//volatile int counter = 0;

void __ISR(_TIMER_3_VECTOR, IPL5SOFT) Timer3ISR(void) 
{
    LED_Invert_A4(); //Motor Direction LED
    
    static int counter = 0;
    static int dir = 1;
    
    counter = counter + dir;
    if (counter > 100)
    {dir = -1;}
    if (counter < 0)
    {dir = 1;}    
   
    //PR2 = 2399, we want the duty cycle to be a percentage of this
    OC4RS = (counter*2399)/100;
    
    IFS0bits.T3IF = 0; //clear interrupt flag
   
    // how many times has the interrupt occurred?
    
    
        //declare a static integer int_num in main
        //int_num++;
        //if (int_num > threshold)
        //int_num 
        //OC1RS = int_num;
    //LED_A4_INV
    // set the duty cycle and direction pin
    
}



int main() {

    __builtin_disable_interrupts();
    initializePIC32(); //set up RAM, vector, JTAG stuff
    LED_Setup_A4();
    IMU_Setup(); // initialize the i2c
    SPI1_init();
    LCD_init();
    
     //Timer 2: 20kHz. PWM Guy.
    T2CONbits.TCKPS = 0; // Timer2 prescaler N=1 (1:1)
    // 48000000 = PBCLK, desired F = 20000
    PR2 = 2399; // PR = PBCLK / N / desiredF - 1
    TMR2 = 0; // initial TMR2 count is 0
          
    //set up output compare on OC4
    //timer 2 is default for OC4
    OC4CONbits.OCM = 0b110; // PWM mode without fault pin; other OC4CON bits are defaults
    OC4RS = 600; // duty cycle, a percentage of PR2
    OC4R = 0; // initialize before turning OC4 on; afterward it is read-only
    //set OC4 to pin RPB13
    RPB13Rbits.RPB13R = 0b0101;
        
    //set up timer 3 for interrupts
    //100hz timer
    T3CONbits.TCKPS = 0b100; //timer3 prescaler of 32
    // 48000000 = PBCLK, desired F = 100
    // PR = PBCLK / N / desiredF - 1
    // 14999 = -1 + 48000000/32/(100) 
    PR3 = 14999;
    TMR3 = 0; //initialize to zero
    IPC3bits.T3IP = 5; //priority 5
    IPC3bits.T3IS = 0; //sub priority 0
    IFS0bits.T3IF = 0; //clear interrupt flag
    IEC0bits.T3IE = 1; //enable interrupt
        
    //turn on
    T2CONbits.ON = 1; // turn on Timer2
    T3CONbits.ON = 1; //turn on timer3
    OC4CONbits.ON = 1; // turn on OC4
    
    
    _CP0_SET_COUNT(0); //begin timer
        
    __builtin_enable_interrupts();    
    LCD_clearScreen(ILI9341_PINK); 
    
    unsigned char red[SCREEN_WIDTH];
    unsigned char green[SCREEN_WIDTH];
    unsigned char blue[SCREEN_WIDTH];
    
   
    
    //for printing to the screen later:
    char m[100];
    int b;   
    
    for (b = 0; b<SCREEN_WIDTH; b++)
    {
        red[b] = b;
        green[b] = 240-b;
        blue[b] = 120;
    }
    b = 0;
   
    LCD_plot_axis(90, 280, 240, 32, ILI9341_BLACK);
    sprintf(m, "RED");
    LCD_print(m, 58, 285, ILI9341_RED, ILI9341_PINK);
    LCD_plot_points(90,280,ILI9341_OLIVE,red,SCREEN_WIDTH);
    sprintf(m, "GREEN");
    LCD_print(m, 108, 285, ILI9341_GREEN, ILI9341_PINK);
    LCD_plot_axis(140, 280, 240, 32, ILI9341_BLACK); 
    LCD_plot_points(140,280,ILI9341_OLIVE,green,SCREEN_WIDTH);
    sprintf(m, "BLUE");
    LCD_print(m, 158, 285, ILI9341_BLUE, ILI9341_PINK);
    LCD_plot_axis(190, 280, 240, 32, ILI9341_BLACK);
    LCD_plot_points(190,280,ILI9341_OLIVE,blue,SCREEN_WIDTH);
        
    //debugging
       
    while(1) 
    {        
//        //blink LED to verify the program hasn't crashed
//        if (_CP0_GET_COUNT() > 24000000)
//        {
//            LED_Invert_A4(); //INVERT LED
//           _CP0_SET_COUNT(0);        
//        }
        
        
        
//        sprintf(m, "b = %3d, red[b] = %3d", b, (int) red[b]);
//        LCD_print(m, 20, 20, ILI9341_BLUE, ILI9341_PINK);
//        
//        b++;
//        if (b > SCREEN_WIDTH)
//        {b=0;}
    }
}