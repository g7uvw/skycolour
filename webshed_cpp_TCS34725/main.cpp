//main.cpp
#include "tcs34725_functions.h"
#include "tcs34725.h"
#include <iostream>
#include <time.h>

void delay_ms(unsigned int delay_ms)
{
  struct timespec tim, tim2;
  tim.tv_sec = 0;
  tim.tv_nsec = delay_ms * 1000000; //10 ms
  nanosleep(&tim , &tim2);
}

using namespace std;
inline void endian_swap(unsigned int& x)
{
    x = (x>>24) |
        ((x<<8) & 0x00FF0000) |
        ((x>>8) & 0x0000FF00) |
        (x<<24);
}
int main()
{
    cout<<"Trying to init the chip"<<endl;

    if (!init(TCS34725_INTEGRATIONTIME_700MS,TCS34725_GAIN_60X))
    //if (!init(TCS34725_INTEGRATIONTIME_2_4MS,TCS34725_GAIN_4X))
        cout<<"Failed to init the chip"<<endl;
    else
        cout<<"That seemed to go ok"<<endl;

    cout<<"Reading sensor"<<endl;

    enable();

while(true)
{
    delay_ms(700);

    uint16_t red = 0,green = 0,blue = 0,clear = 0;
    uint32_t rgb = 0;
    tie(red,green,blue,clear) = get_raw_data();

    //cout<<"Raw values: "<<endl;
    //cout<<"Red = "<<red<<endl;
    //cout<<"Green = "<<green<<endl;
    //cout<<"Blue = "<<blue<<endl;
    //cout<<"clear = "<<clear<<endl;
    rgb = (red/32 * 65536) + (green/32 * 256) + blue/32;
    cout<<"RGB Colour: " << std::hex << rgb << endl;
}



return 0;
}
