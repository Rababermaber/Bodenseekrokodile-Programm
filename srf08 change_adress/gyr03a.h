#ifndef I2C_GYRO_H
#define I2C_GYRO_H
#include "stddefs.h"  
#define GYROADRESS (0b1101011 << 1)

//test
extern uint16_t x_out;
extern uint16_t y_out;
extern uint16_t z_out;

void Gyro_byte (void);

#endif
