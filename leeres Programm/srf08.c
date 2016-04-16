#include "srf08.h"
#include "twimaster.h"	
#include <util/delay.h>

/************************************************************************/
/*************      SRF08 FUNKTIONEN - selbstgeschrieben G. Kaestle*************/
/************************************************************************/
//Prüfen, ob Sensor bereit zum Auslesen ist
uint8_t srf08_ready(unsigned char srf08adress)
{

	if(!(i2c_start(srf08adress+I2C_WRITE)))
		{	i2c_stop();
			return 1;
		}	
	i2c_stop();
	return 0;
}
//verstärkung und ´Maximalabstand setzen
unsigned char US_init(unsigned char srf08adress)
{  
	if(i2c_start(srf08adress+I2C_WRITE))return 0;
	i2c_write(0x01);//Gainregister
	i2c_write(23);//Gain setzen
	//i2c_write(140); // ca. 40ms
	i2c_stop();
	
	if(i2c_start(srf08adress+I2C_WRITE))return 0;
	i2c_write(0x02); // Reichweitenregister
	i2c_write(0x30); // knapp über 2m (43mm x 48) ca. 16ms
	i2c_stop();
	
	return 1;
}

unsigned char US_light(unsigned char srf08adress) {
	uint16_t SRF08_messwert;
	 //srf08_ready(srf08adress);
	 while(i2c_start(srf08adress+I2C_WRITE))i2c_stop();
	 i2c_write(0x02);//Ergebnisregister Abstand wählen
	 i2c_stop();
	 while(i2c_rep_start(srf08adress+I2C_READ));
	 
	SRF08_messwert = i2c_read(0x01);

	i2c_stop();

 	return SRF08_messwert;

}


//messung starten
unsigned char US_start(unsigned char srf08adress)
{  
	if(i2c_start(srf08adress+I2C_WRITE))return 0;
	i2c_write(0x01);//Gainregister
	i2c_write(0x0);//Gain setzen
	i2c_write(0x30);
	
	i2c_rep_start(srf08adress+I2C_WRITE);
	i2c_write(0x00);//Befehlsregister wählen		
	i2c_write(0x51);//Messung starten, Ergebnis in cm
		
	i2c_stop();
	return 1;
}
unsigned int US_read(unsigned char srf08adress)
{
	uint16_t SRF08_messwert;
	 //srf08_ready(srf08adress);
	 while(i2c_start(srf08adress+I2C_WRITE))i2c_stop();
	 i2c_write(0x02);//Ergebnisregister Abstand wählen
	 i2c_stop();
	 while(i2c_rep_start(srf08adress+I2C_READ));
	 
	SRF08_messwert = ((i2c_readAck())<<8); //highbyte
	SRF08_messwert += (i2c_readNak()); //lowbyte
	i2c_stop();

 	return SRF08_messwert;
}


unsigned int US_distance3(unsigned char srf08adress)
{  
  	US_start(srf08adress);

	uint16_t SRF08_messwert;


	 while(srf08_ready(srf08adress));
	 while(i2c_start(srf08adress+I2C_WRITE))i2c_stop();
	 i2c_write(0x02);//Ergebnisregister Abstand wählen
	 i2c_stop();
	 while(i2c_rep_start(srf08adress+I2C_READ));
	 
				
	//SRF08_light= i2c_readAck();
	SRF08_messwert = ((i2c_readAck())<<8); //highbyte
	SRF08_messwert += (i2c_readNak()); //lowbyte
	i2c_stop();
 	return SRF08_messwert;
}

void change_i2c_address_srf08 (unsigned char old_address, unsigned char new_address)
// change i2c-address of srf08  ##getestet
{ 
    i2c_start(old_address+I2C_WRITE);       
    i2c_write(0x00);
    i2c_write(0xA0);
	i2c_stop();
	
	_delay_ms(50);
	
	i2c_start(old_address+I2C_WRITE);                      
    i2c_write(0x00);   
    i2c_write(0xAA);   
	i2c_stop();
	
	_delay_ms(50);
	
	i2c_start(old_address+I2C_WRITE);                      
    i2c_write(0x00);  
    i2c_write(0xA5);  
	i2c_stop();
	
	_delay_ms(50);
	
	i2c_start(old_address+I2C_WRITE);                      
    i2c_write(0x00);                      
    i2c_write(new_address);                      
    i2c_stop();
}

/************************************************************************/
/*************      SRF08 FUNKTIONEN  Ende                  *************/
/************************************************************************/
