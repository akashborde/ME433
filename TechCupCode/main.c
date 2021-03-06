//#include<xc.h>           // processor SFR definitions
//#include<sys/attribs.h>  // __ISR macro
//#include <stdio.h>
//
//// DEVCFG0
//#pragma config DEBUG = OFF // no debugging
//#pragma config JTAGEN = OFF // no jtag
//#pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
//#pragma config PWP = OFF // no write protect
//#pragma config BWP = OFF // no boot write protect
//#pragma config CP = OFF // no code protect
//
//// DEVCFG1
//#pragma config FNOSC = PRIPLL // use primary oscillator with pll
//#pragma config FSOSCEN = OFF // turn off secondary oscillator
//#pragma config IESO = OFF // no switching clocks
//#pragma config POSCMOD = HS // high speed crystal mode
//#pragma config OSCIOFNC = OFF // disable secondary osc
//#pragma config FPBDIV = DIV_1 // divide sysclk freq by 1 for peripheral bus clock
//#pragma config FCKSM = CSDCMD // do not enable clock switch
//#pragma config WDTPS = PS1048576 // use slowest wdt
//#pragma config WINDIS = OFF // wdt no window mode
//#pragma config FWDTEN = OFF // wdt disabled
//#pragma config FWDTWINSZ = WINSZ_25 // wdt window at 25%
//
//// DEVCFG2 - get the sysclk clock to 48MHz from the 8MHz crystal
//#pragma config FPLLIDIV = DIV_2 // divide input clock to be in range 4-5MHz
//#pragma config FPLLMUL = MUL_24 // multiply clock after FPLLIDIV
//#pragma config FPLLODIV = DIV_2 // divide clock after FPLLMUL to get 48MHz
//#pragma config UPLLIDIV = DIV_2 // divider for the 8MHz input clock, then multiplied by 12 to get 48MHz for USB
//#pragma config UPLLEN = ON // USB clock on
//
//// DEVCFG3
//#pragma config USERID = 0 // some 16bit userid, doesn't matter what
//#pragma config PMDL1WAY = ON // allow multiple reconfigurations
//#pragma config IOL1WAY = ON // allow multiple reconfigurations
//#pragma config FUSBIDIO = OFF // USB pins controlled by USB module
//#pragma config FVBUSONIO = OFF // USB BUSON controlled by USB module
//
//
//#include "ili9341.h"
//
//#define DIR1 LATAbits.LATA10
//#define DIR2 LATAbits.LATA7
//#define USER PORTBbits.RB4
//
//
//
//void startup() {
//    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
//    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);
//
//    // 0 data RAM access wait states
//    BMXCONbits.BMXWSDRM = 0x0;
//
//    // enable multi vector interrupts
//    INTCONbits.MVEC = 0x1;
//
//    // disable JTAG to get pins back
//    DDPCONbits.JTAGEN = 0;
//
//    // pin definitions
//    ANSELA = 0;
//    ANSELB = 0;
//    TRISAbits.TRISA7 = 0; // DIR2
//    DIR2 = 1;
//    TRISAbits.TRISA10 = 0; // DIR1
//    DIR1 = 1;
//    TRISBbits.TRISB4 = 1; // USER
//    
//    // OC1 is B15, goes with DIR1
//    
//    // OC4 is A4, goes with DIR2
//    
//    // LCD uses SPI1: A0 is SDO, A1 is SDI, B5 is CST, B14 is SCK1, A9 is DC, B7 is CS
//    SPI1_init();
//    LCD_init();
//    LCD_clearScreen(ILI9341_DARKGREEN);
//    
//    // Camera uses C0-7, C8 as OC2, A8 as INT3/PCLK, B13 as INT2/HREF, C9 as INT1/VSYNC, and I2C1
//    
//    // B3 is available as SCL2, B2 is available as SDA2
//    
//    
//    
//    
//}
//
//int main() {
//
//    __builtin_disable_interrupts();
//
//    startup();
//
//    __builtin_enable_interrupts();
//    
//    int I = 0;
//    char message[100];
//    
//       
//    while(1) {
//
//        _CP0_SET_COUNT(0);
//        while(_CP0_GET_COUNT()<48000000/2/2){
//            while(USER == 0){}
//        }
//        DIR1 = 0;
//        DIR2 = 1;
//        
//        _CP0_SET_COUNT(0);
//        while(_CP0_GET_COUNT()<48000000/2/2){
//            while(USER == 0){}
//        }
//        DIR1 = 1;
//        DIR2 = 0;
//        
//        I++;
//        sprintf(message,"I = %d   ", I);
//        drawString(140,92,message);
//
//    }
//}
//


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
//#include <math.h>

//#include "i2c_master_noint.h" //i2c initialization File

//values for #pragma config settings are found in:
// /opt/microchip/xc32/v2.15/docs/config_docs

//volatile int counter = 0;

void __ISR(_TIMER_3_VECTOR, IPL5SOFT) Timer3ISR(void) 
{
    LED_Invert_A4(); //Motor Direction LED
    
//    static int counter = 0;
//    static int dir = 1;
//    
//    counter = counter + dir;
//    if (counter > 100)
//    {dir = -1;}
//    if (counter < 0)
//    {dir = 1;}    
   
    int percentage = 50;
    //PR2 = 2399, we want the duty cycle to be a percentage of this
    OC4RS = (percentage*2399)/100;
    OC1RS = (percentage*2399)/100;
    
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
    //LED_Setup_A4();
//    IMU_Setup(); // initialize the i2c
    SPI1_init();
    LCD_init();
//    
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
    //set OC4 to pin RPA4
    RPA4Rbits.RPA4R = 0b0101;
    
    //set up OC1 for the other wheel
    OC1CONbits.OCM = 0b110; //same
    OC1RS = 600; //duty cycle, a percentage of PR2
    OC1R = 0; //initialize before turning oc1 on, afterwards its just read only
    RPB15Rbits.RPB15R = 0b0101; //OC1
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
    OC1CONbits.ON = 1; //TURN ON OC1
//    
//    
    _CP0_SET_COUNT(0); //begin timer
        
    __builtin_enable_interrupts();    
    LCD_clearScreen(ILI9341_PINK); 
    
//    unsigned char red[SCREEN_WIDTH];
//    unsigned char green[SCREEN_WIDTH];
//    unsigned char blue[SCREEN_WIDTH];
//    
//   
//    
//    //for printing to the screen later:
//    char m[100];
//    int b;   
//    
//    for (b = 0; b<SCREEN_WIDTH; b++)
//    {
//        red[b] = b;
//        green[b] = 240-b;
//        blue[b] = 120;
//    }
//    b = 0;
//   
//    LCD_plot_axis(90, 280, 240, 32, ILI9341_BLACK);
//    sprintf(m, "RED");
//    LCD_print(m, 58, 285, ILI9341_RED, ILI9341_PINK);
//    LCD_plot_points(90,280,ILI9341_OLIVE,red,SCREEN_WIDTH);
//    sprintf(m, "GREEN");
//    LCD_print(m, 108, 285, ILI9341_GREEN, ILI9341_PINK);
//    LCD_plot_axis(140, 280, 240, 32, ILI9341_BLACK); 
//    LCD_plot_points(140,280,ILI9341_OLIVE,green,SCREEN_WIDTH);
//    sprintf(m, "BLUE");
//    LCD_print(m, 158, 285, ILI9341_BLUE, ILI9341_PINK);
//    LCD_plot_axis(190, 280, 240, 32, ILI9341_BLACK);
//    LCD_plot_points(190,280,ILI9341_OLIVE,blue,SCREEN_WIDTH);
//        
//    //debugging
       
    while(1) 
    {        
//        //blink LED to verify the program hasn't crashed
//        if (_CP0_GET_COUNT() > 24000000)
//        {
//            LED_Invert_A4(); //INVERT LED
//           _CP0_SET_COUNT(0);        
//        }
//        
        
        
//        sprintf(m, "b = %3d, red[b] = %3d", b, (int) red[b]);
//        LCD_print(m, 20, 20, ILI9341_BLUE, ILI9341_PINK);
//        
//        b++;
//        if (b > SCREEN_WIDTH)
//        {b=0;}
    }
}