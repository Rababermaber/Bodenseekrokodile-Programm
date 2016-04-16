#include "mpu650.h"
#include "twimaster.h"
#include "stddefs.h"
#include "util/delay.h"
int16_t yaw = 0;
int16_t pitch = 0;
int16_t roll = 0;
int16_t acc_x=0;
int16_t acc_y=0;
int16_t acc_z=0;


void gyro_init()
{
	i2c_start(GYROADRESS+I2C_WRITE);
	i2c_write(0x6B);
	i2c_write(0x80);
	i2c_stop();
	_delay_ms(20);
	i2c_start(GYROADRESS+I2C_WRITE);
	i2c_write(0x6B);
	i2c_write(0x00);
	i2c_stop();
	i2c_start(GYROADRESS+I2C_WRITE);
	i2c_write(0x1A);
	i2c_write(0x06);
	i2c_write(0x18);
	i2c_write(0x00);
	i2c_stop();
	i2c_start(GYROADRESS+I2C_WRITE);
	i2c_write(0x1C);
	i2c_write(0b10<<3);
	i2c_stop();
	// Who am I
	//i2c_write(0x0F);
	//i2c_rep_start(GYROADRESS+I2C_READ);
	//yaw=i2c_readNak();

}

void Gyro_byte ()
// ########### nicht gestestet.
{
    if(!i2c_start(GYROADRESS+I2C_WRITE)) 
	{
    i2c_write(0x43);
	}
    if(!i2c_rep_start(GYROADRESS+I2C_READ))
	{

	yaw =  (i2c_readAck() << 8);
	yaw+=i2c_readAck();
	yaw*=0.061035156;
	yaw-=27;

	pitch= (i2c_readAck() << 8);
	pitch+= i2c_readAck();
	pitch*=0.061035156;
	pitch-=1;

	roll= (i2c_readAck() << 8);
	roll+= i2c_readNak();
	roll*=0.061035156;
	}
	i2c_stop();

}

void acc_values()
{
	if(!i2c_start(GYROADRESS+I2C_WRITE)) 
	{
		i2c_write(0x3B);
	}
	if(!i2c_rep_start(GYROADRESS+I2C_READ))
	{
		acc_x = (i2c_readAck() << 8);
		acc_x += i2c_readAck();
		acc_x *= 0.061035156;
		acc_x -= 23;
		
		acc_y = (i2c_readAck() << 8);
		acc_y += i2c_readAck();
		acc_y *= 0.061035156;
		
		acc_z = (i2c_readAck() << 8);
		acc_z += i2c_readNak();
		acc_z *= 0.061035156;
	}
	i2c_stop();
}
