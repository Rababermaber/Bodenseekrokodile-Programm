#ifndef I2C_US_H
#define I2C_US_H
#include "stddefs.h"  


/************************************************************************/
/*************      SRF08 FUNKTIONEN - selbstgeschrieben G. Kaestle*************/
/************************************************************************/
//Prüfen, ob Sensor bereit zum Auslesen ist
uint8_t srf08_ready(unsigned char srf08adress);

unsigned char US_init(unsigned char srf08adress);

unsigned char US_start(unsigned char srf08adress);

unsigned int US_read(unsigned char srf08adress);

unsigned int US_distance3(unsigned char srf08adress);

void change_i2c_address_srf08 (unsigned char old_address, unsigned char new_address);


/************************************************************************/
/*************      SRF08 FUNKTIONEN  Ende                  *************/
/************************************************************************/

#endif
