/* 
 * File:   pragmas.h
 * Author: akashborde
 *
 * Created on April 16, 2019, 3:41 PM
 */

#ifndef PRAGMAS_H
#define	PRAGMAS_H

#ifdef	__cplusplus
extern "C" {
#endif

    /* The source of these pragmas is from HW1
     * Found the HTML file corresponding to this PIC and used that datasheet
     * //values for #pragma config settings are found in:
     *   // /opt/microchip/xc32/v2.15/docs/config_docs
     */

    // DEVCFG0
    #pragma config DEBUG = OFF // no debugging
    #pragma config JTAGEN = OFF // no jtag
    #pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
    #pragma config PWP = OFF // no write protect
    #pragma config BWP = OFF // no boot write protect
    #pragma config CP = OFF // no code protect

    // DEVCFG1
    #pragma config FNOSC = PRIPLL // use primary oscillator with pll
    #pragma config FSOSCEN = OFF // turn off secondary oscillator
    #pragma config IESO = OFF // no switching clocks
    #pragma config POSCMOD = HS // high speed crystal mode
    #pragma config OSCIOFNC = OFF // disable secondary osc
    #pragma config FPBDIV = DIV_1 // divide sysclk freq by 1 for peripheral bus clock
    #pragma config FCKSM = CSDCMD // do not enable clock switch
    #pragma config WDTPS = PS1 // use slowest wdt
    #pragma config WINDIS = OFF // wdt no window mode
    #pragma config FWDTEN = OFF // wdt disabled
    #pragma config FWDTWINSZ = WINSZ_25  // wdt window at 25%

    // DEVCFG2 - get the sysclk clock to 48MHz from the 8MHz crystal
    #pragma config FPLLIDIV = DIV_2 // divide input clock to be in range 4-5MHz
    #pragma config FPLLMUL = MUL_24 // multiply clock after FPLLIDIV, 4*24 = 96MHz
    #pragma config FPLLODIV = DIV_2 // divide clock after FPLLMUL, 96/2 = 48MHz
    #pragma config UPLLIDIV = DIV_2 // divider for the 8MHz input clock, then multiplied by 12 to get 48MHz for USB
    #pragma config UPLLEN = ON // USB clock on

    // DEVCFG3
    #pragma config USERID = 00000000 // some 16bit userid, doesn't matter what
    #pragma config PMDL1WAY = OFF // allow multiple reconfigurations
    #pragma config IOL1WAY = OFF // allow multiple reconfigurations
    #pragma config FUSBIDIO = ON // USB pins controlled by USB module
    #pragma config FVBUSONIO = ON // USB BUSON controlled by USB module

#ifdef	__cplusplus
}
#endif

#endif	/* PRAGMAS_H */

