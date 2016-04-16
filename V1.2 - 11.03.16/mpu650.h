#ifndef I2C_GYRO_H
#define I2C_GYRO_H
#include "stddefs.h"  
#define GYROADRESS 0xD0

//test
extern int16_t yaw;
extern int16_t pitch;
extern int16_t roll;
extern int16_t acc_x;
extern int16_t acc_y;
extern int16_t acc_z;
void gyro_init(void);
void Gyro_byte (void);
void acc_values(void);

#endif
