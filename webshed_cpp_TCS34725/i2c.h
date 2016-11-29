//i2c.h

#ifndef _I2C_H
#define _I2C_H

#include <stdint.h>
#include "tcs34725.h"

bool OpenBus(void);
bool CloseBus(void);

uint8_t readU8(uint8_t reg);
uint16_t readU16LE(uint8_t reg);
bool write8(uint8_t reg, uint8_t value);



#endif