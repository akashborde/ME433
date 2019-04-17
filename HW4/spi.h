/* 
 * File:   spi.h
 * Author: akashborde
 *
 * Created on April 16, 2019, 4:28 PM
 */

#ifndef SPI_H
#define	SPI_H

#ifdef	__cplusplus
extern "C" {
#endif

    
    void initSPI1(); // initalizes SPI1 channel
    unsigned char SPI1_IO(unsigned char write); //generic communication
    void setVoltage(char channel, int voltage);



#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

