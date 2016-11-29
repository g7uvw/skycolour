//i2c.cpp
#include "i2c.h"
#include <stdio.h>
#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port

char *filename = (char*)"/dev/i2c-1";
int file_i2c;

bool OpenBus(void)
{
	if ((file_i2c = open(filename, O_RDWR)) < 0)
	     {
              printf("Failed to open the i2c bus");
		      return false;
	     }

	if (ioctl(file_i2c, I2C_SLAVE, TCS34725_ADDRESS) < 0)
	     {
		      printf("Failed to acquire bus access and/or talk to slave.\n");
		      //ERROR HANDLING; you can check errno to see what went wrong
		      return false;
	     }
    return true;
}

bool CloseBus(void)
{
    return close(file_i2c);
}


uint8_t readU8(uint8_t reg)
{
    //open the I2C Bus
    //write to the filehandle the address we want to read from
    //read one byte
    //closebus

    uint8_t data;
    uint8_t _register = TCS34725_COMMAND_BIT | reg;

    if(!OpenBus())
        return NULL;

    //if(!write(file_i2c,&reg,1))
		if(!write(file_i2c,&_register,1))
        return NULL;

    if (read(file_i2c, &data, 1) != 1)
	{
		//ERROR HANDLING: i2c transaction failed
		printf("Failed to read from the i2c bus.\n");
        return NULL;
	}

    CloseBus();

    return data;
}

uint16_t readU16LE(uint8_t reg)
{

    //open the I2C Bus
    //write to the filehandle the address we want to read from
    //read two bytes
    //closebus

    uint16_t data;
		uint8_t _register = TCS34725_COMMAND_BIT | reg;

    if(!OpenBus())
        return NULL;

    if(!write(file_i2c,&_register,1))
        return NULL;

    if (read(file_i2c, &data, 2) != 2)
	{
		//ERROR HANDLING: i2c transaction failed
		printf("Failed to read from the i2c bus.\n");
        return NULL;
	}

    CloseBus();

    return data;
}

bool write8(uint8_t reg, uint8_t value)
{
    //open the I2C Bus
    //write to the filehandle the address we want to read from
    //write one byte
    //closebus
		uint8_t _register = TCS34725_COMMAND_BIT | reg;
    if(!OpenBus())
        return NULL;

    if(!write(file_i2c,&_register,1))
        return NULL;

    if (write(file_i2c, &value, 1) != 1)		//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
	     {
		  /* ERROR HANDLING: i2c transaction failed */
		  printf("Failed to write to the i2c bus.\n");
          return NULL;
	     }
    return true;
}
