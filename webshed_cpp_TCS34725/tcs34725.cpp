// tcs34725.cpp

//TODO CLASSisy this
#include <stdint.h>
#include "tcs34725_functions.h"
#include "i2c.h"
#include <time.h>
#include <math.h>

int calculate_color_temperature(double r, double g, double b)
{
//# 1. Map RGB values to their XYZ counterparts.
//    # Based on 6500K fluorescent, 3000K fluorescent
//    # and 60W incandescent values for a wide range.
//    # Note: Y = Illuminance or lux
    double X = (-0.14282 * r) + (1.54924 * g) + (-0.95641 * b);
    double Y = (-0.32466 * r) + (1.57837 * g) + (-0.73191 * b);
    double Z = (-0.68202 * r) + (0.77073 * g) + ( 0.56332 * b);
//    # Check for divide by 0 (total darkness) and return None.
    if ((X + Y + Z) == 0)
        return 0;
//    # 2. Calculate the chromaticity co-ordinates
    double xc = (X) / (X + Y + Z);
    double yc = (Y) / (X + Y + Z);
//    # Check for divide by 0 again and return None.
    if ((0.1858 - yc) == 0)
        return 0;
//    # 3. Use McCamy's formula to determine the CCT
    double n = (xc - 0.3320) / (0.1858 - yc);
//    # Calculate the final CCT
    double cct = (449.0 * pow(n,3.0)) + (3525.0 * pow(n,2.0)) + (6823.3 * n) + 5520.33;
    return (int)cct;
}


int calculate_lux(double r, double g, double b)
{
//"""Converts the raw R/G/B values to luminosity in lux."""
    double illuminance = (-0.32466 * r) + (1.57837 * g) + (-0.73191 * b);
return (int)illuminance;
}



bool enable(void)
{
// # Flip on the power and enable bits.
    write8(TCS34725_ENABLE, TCS34725_ENABLE_PON);
    struct timespec tim, tim2;
    tim.tv_sec = 0;
    tim.tv_nsec = 10000000; //10 ms
    nanosleep(&tim , &tim2);
    return write8(TCS34725_ENABLE, (TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN));
}


bool disable(void)
{
//    # Flip off the power on and enable bits.
        uint8_t reg = readU8(TCS34725_ENABLE);
        reg &= ~(TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN);
        return write8(TCS34725_ENABLE, reg);
}

bool init(uint8_t integration_time, uint8_t gain)
{

    //check we can read the chip ID
    uint8_t ID = readU8(TCS34725_ID);
    if ((ID != 0x44) && (ID != 0x10))
        return false;
    if(!set_integration_time(integration_time))
        return false;
    if(!set_gain(gain))
        return false;
    if(!enable())
        return false;

        struct timespec tim, tim2;
        tim.tv_sec = 0;
        tim.tv_nsec = 100000000; //100 ms
        nanosleep(&tim , &tim2);
    return true;
}


bool set_integration_time(uint8_t integration_time)
{
    return write8(TCS34725_ATIME, integration_time);
}

uint8_t get_intergration_time(void)
{
    return readU8(TCS34725_ATIME);
}

bool set_gain(uint8_t gain)
{
    return write8(TCS34725_CONTROL, gain);
}

uint8_t get_gain(void)
{
    return readU8(TCS34725_CONTROL);
}


tuple<uint16_t, uint16_t, uint16_t, uint16_t> get_raw_data(void)
{

//Reads the raw red, green, blue and clear channel values. Will return
//        a 4-tuple with the red, green, blue, clear color values (unsigned 16-bit
//        numbers).
//
//        # Read each color register.
    uint16_t  r = readU16LE(TCS34725_RDATAL);
    uint16_t  g = readU16LE(TCS34725_GDATAL);
    uint16_t  b = readU16LE(TCS34725_BDATAL);
    uint16_t  c = readU16LE(TCS34725_CDATAL);

    return  make_tuple(r, g, b, c);

}

bool clear_interrupt(void);
//bool set_interrupt_limits(low, high):
