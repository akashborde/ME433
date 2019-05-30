/* 
 * File:   peripheral.h
 * Author: akashborde
 *
 * Created on April 24, 2019, 5:19 PM
 */

#ifndef PERIPHERAL_H
#define	PERIPHERAL_H

#ifdef	__cplusplus
extern "C" {
#endif

#define TEN_HZ_PERIOD (4807692 / 2) //# of counts needed for core counts to overflow
    //at 0.1sec
#define SCREEN_WIDTH 240

    
void initializePIC32(void); // initializes pic
void LED_Setup_A4(void); //sets up led on A4 and turns it on
void LED_Invert_A4(void); //inverts led



#ifdef	__cplusplus
}
#endif

#endif	/* PERIPHERAL_H */

