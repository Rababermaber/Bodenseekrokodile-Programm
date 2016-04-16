#ifndef I2C_BNO_H
#define I2C_BNO_H
#include "stddefs.h"  
#define BNOADDRESS 0x50

//test

void bno_init(void);
void bno_start_ndof(void);
int16_t bno_degree(uint8_t axis);
float bno_linear_acceleration(uint8_t axis);
int16_t bno_dps(uint8_t axis);
uint8_t bno_read_system_info(uint8_t data_register);

#endif
