// TCS34725_functions.h

#ifndef _TCS34725_functions_H
#define _TCS34725_functions_H

#include <tuple>
#include <stdint.h>
#include "tcs34725.h"
using namespace std;

int calculate_color_temperature(double r, double g, double b);
int calculate_lux(double r, double g, double b);
bool enable(void);
bool disable(void);
bool init(uint8_t integration_time, uint8_t gain);
bool set_integration_time(uint8_t integration_time);
uint8_t get_intergration_time(void);
bool set_gain(uint8_t gain);
uint8_t get_gain(void);

tuple<uint16_t, uint16_t, uint16_t, uint16_t> get_raw_data(void);
bool clear_interrupt(void);
//bool set_interrupt_limits(low, high):

#endif 