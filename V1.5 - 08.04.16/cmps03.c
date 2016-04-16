#include "cmps03.h"
#include "twimaster.h"	
#include "stddefs.h"  

/************************************************************************/
/*************      CMPS03-Kompass FUNKTIONEN - selbstgeschrieben G. Kaestle*************/
/************************************************************************/

uint8_t CMPS_byte ()
// ########### nicht gestestet.
{  
   uint8_t z;
    i2c_start(cmps03+I2C_WRITE);       
    i2c_write(0x01);                       // Register 1 enthält den Kompasswert als byte-Wert 0-255
    i2c_stop();                            // set stop conditon = release bus
    
	while (i2c_start(cmps03+I2C_READ))
	 {
	 i2c_stop(); 
	 }  
  	z=i2c_readNak();
    i2c_stop();
	return z;
}

 
/************************************************************************/
/*************      CMPS03-Kompass FUNKTIONEN  Ende  *************/
/************************************************************************/

uint16_t CMPS_grad()
{
  unsigned char z0;
  unsigned char z1;
  
  uint16_t result;
  
    i2c_start(cmps03+I2C_WRITE);       
    i2c_write(0x02);                       // Register 1 enthält den Kompasswert als byte-Wert 0-255
    i2c_stop();                            // set stop conditon = release bus
    
	while (i2c_start(cmps03+I2C_READ))
	 {
	 i2c_stop(); 
	 }  
  	z0=i2c_readAck();
	z1=i2c_readNak();
    i2c_stop();
	
	result = 256*z0+z1;
	return (result);
}

int16_t relativeRichtung(uint16_t Nullrichtung)
{
  int16_t rel = CMPS_grad()-Nullrichtung;
  
  if (rel<(-1800)) {rel += 3600;}
  if (rel>1799) {rel -= 3600;}
  return rel;
}
