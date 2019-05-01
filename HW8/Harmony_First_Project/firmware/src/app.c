/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include "app.h"
#include "ili9341.h"
#include "i2c_master_noint.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;
    
    /*
     * In the APP_Initialize function, place your pin initializations.
     */
    
    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);
    
     // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;

    // do your TRIS and LAT commands here
     
    // Pushbutton pin: B4
    TRISBbits.TRISB4 = 1; // Set it as an input
        
    // Green LED pin:  A4
    TRISAbits.TRISA4 = 0; //set as output
    LATAbits.LATA4 = 1;   //initialized as high
    
    
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            bool appInitialized = true;
       
        
            if (appInitialized)
            {
            
                appData.state = APP_STATE_SERVICE_TASKS;
                
            }
            break;
        }

        case APP_STATE_SERVICE_TASKS:
        {
            
            IMU_Setup(); // initialize the i2c
            SPI1_init();
            LCD_init();
            _CP0_SET_COUNT(0); //begin timer
            
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
                    if(_CP0_GET_COUNT() > 40000)
                    {
                        LATAINV = 0b10000; //INVERT THE LED
                        _CP0_SET_COUNT(0);
                    }
                    IMU_read_multiple(OUT_TEMP_L, dat, NUM_READS);
            
/*            temp =      (dat[1] << 8) | dat[0];
            x_angle =   (dat[3] << 8) | dat[2];
            y_angle =   (dat[5] << 8) | dat[4];
            z_angle =   (dat[7] << 8) | dat[6];
*/
                    x_accel =   (dat[9] << 8) | dat[8];
                    y_accel =   (dat[11] << 8) | dat[10];
                    z_accel =   (dat[13] << 8) | dat[12];
            
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
                else
                {
                    sprintf(m, "Can't read from IMU");
                    LCD_print(m, START_X, START_Y, ILI9341_BLUE, ILI9341_PINK);
                }
            }
            break;
        }

        /* TODO: implement your application state machine.*/
        

        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}

 

/*******************************************************************************
 End of File
 */
