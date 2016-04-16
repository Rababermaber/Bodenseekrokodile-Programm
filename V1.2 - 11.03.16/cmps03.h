#ifndef I2C_CMPS_H
#define I2C_CMPS_H
#include "stddefs.h"  


/************************************************************************/
/*************      CMPS03-Kompass FUNKTIONEN - selbstgeschrieben G. Kaestle*************/
/************************************************************************/

uint8_t CMPS_byte (void);

 
/************************************************************************/
/*************      CMPS03-Kompass FUNKTIONEN  Ende  *************/
/************************************************************************/

uint16_t CMPS_grad(void);

int16_t relativeRichtung(uint16_t Nullrichtung);



#endif
