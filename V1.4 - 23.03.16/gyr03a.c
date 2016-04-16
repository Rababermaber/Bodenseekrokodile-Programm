#include "gyr03a.h"
#include "twimaster.h"
#include "stddefs.h"

uint16_t x_out = 0;
uint16_t y_out = 0;
uint16_t z_out = 0;


void gyro_init()
{
	i2c_start(GYROADRESS+I2C_WRITE);
	i2c_write(0x20);  
	i2c_write(0x0F);   
	i2c_stop();


	// Who am I
	//i2c_write(0x0F);
	//i2c_rep_start(GYROADRESS+I2C_READ); 
	//x_out=i2c_readNak();
	
}

void Gyro_byte ()
// ########### nicht gestestet.
{  
    i2c_start(GYROADRESS+I2C_WRITE);       
    i2c_write(0x28);                       
    i2c_rep_start(GYROADRESS+I2C_READ);   
	x_out=i2c_readAck();
	x_out = x_out + (i2c_readNak() << 8);
	
	i2c_start(GYROADRESS+I2C_WRITE);       
    i2c_write(0x2A);                       
    i2c_rep_start(GYROADRESS+I2C_READ);   
	y_out=i2c_readAck();
	y_out = y_out + (i2c_readNak() << 8);
	
	i2c_start(GYROADRESS+I2C_WRITE);       
    i2c_write(0x2C);                       
    i2c_rep_start(GYROADRESS+I2C_READ);   
	z_out=i2c_readAck();
	z_out = z_out + (i2c_readNak() << 8);
	
	i2c_stop();

}

 


