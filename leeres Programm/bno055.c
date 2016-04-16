#include "bno055.h"
#include "twimaster.h"
#include "stddefs.h"
#include "util/delay.h"

int16_t bno_read_sensor_data(uint8_t data_register)
{
	int16_t ret=-1;
	if(!i2c_start(BNOADDRESS+I2C_WRITE))
		i2c_write(data_register);
	
	
	if(!i2c_rep_start(BNOADDRESS+I2C_READ))
	{
		ret = i2c_readAck();			
		ret += (i2c_readNak()<<8);      
	}
	i2c_stop();
	return ret;
}

uint8_t bno_read_system_info(uint8_t data_register) 
{
	uint8_t ret=-1;
	if(!i2c_start(BNOADDRESS+I2C_WRITE))
		i2c_write(data_register);	
	
	if(!i2c_rep_start(BNOADDRESS+I2C_READ))
		ret = i2c_readNak();
	i2c_stop();
	return ret;
	
}

int16_t bno_degree(uint8_t axis)
{
	return bno_read_sensor_data(axis)/16;
}

float bno_linear_acceleration(uint8_t axis)  //float / mg =LSB, da m/s² niedrige Auflösung
{
	return bno_read_sensor_data(axis)/100.0;
}

int16_t bno_dps(uint8_t axis)
{
	return bno_read_sensor_data(axis)/10;
}

void bno_start_ndof()
{
	if(bno_read_system_info(BNO_OPMODE)!=12)
	{
		i2c_start(BNOADDRESS+I2C_WRITE);
		i2c_write(0x3D);
		i2c_write(0x0C); //Choose operation Mode (standard NDOF)
		i2c_stop();
		i2c_start(BNOADDRESS+I2C_WRITE);
		i2c_write(0x3F);
		i2c_write(0x01); //Self test
		i2c_stop();
	}
}

void bno_init()
{
	i2c_start(BNOADDRESS+I2C_WRITE);
	i2c_write(0x3F);
	i2c_write(0x20); //System_Reset
	i2c_stop();
}